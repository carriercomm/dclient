#include <cstring>

#include <boost/lexical_cast.hpp>

#include <fcntl.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#include "adt/logs.h"

#include "deliantra/communicator.h"
#include "deliantra/exception.h"

static std::string
strerror ()
{
  return strerror (errno);
}

communicator::communicator (std::string const &server, std::string const &port)
  : response_len (0)
  , response { 0 }
{
  LOG ("net") << "creating socket\n";
  int sockfd = socket (AF_INET, SOCK_STREAM, 0);
  if (sockfd == -1)
    throw "socket: " + strerror ();

  int portno = boost::lexical_cast<int> (port);

  LOG ("net") << "resolving hostname: " << server << "\n";
  hostent *serverent = gethostbyname (server.c_str ());
  if (!serverent)
    throw "gethostbyname: " + strerror ();

  LOG ("net") << "connecting to: " << server << ":" << portno << "\n";
  sockaddr_in addr = { 0 };
  addr.sin_family = AF_INET;
  memcpy (&addr.sin_addr.s_addr, serverent->h_addr, serverent->h_length);
  addr.sin_port = htons (portno);
  if (connect (sockfd, reinterpret_cast<sockaddr const *> (&addr), sizeof addr) == -1)
    throw "connect: " + strerror ();

  LOG ("net") << "setting socket to non-blocking\n";
  int flags = fcntl (sockfd, F_GETFL, 0);
  if (flags == -1)
    flags = 0;
  if (fcntl (sockfd, F_SETFL, flags | O_NONBLOCK) == -1)
    throw "fcntl: " + strerror ();

  LOG ("net") << "setting up read callback\n";
  sock.set (sockfd, ev::READ);
  sock.set<communicator, &communicator::read_cb> (this);
  sock.start ();
}

void
communicator::set_packet_handler (interface::packet_handler &handler)
{
  on = &handler;
}

void
communicator::read_cb (ev::io &w, int revents)
{
  //LOG ("net") << "data is ready\n";

  ssize_t readlen = read (w.fd, response + response_len, sizeof response - response_len);
  if (readlen < 0)
    throw "read: " + strerror ();
  if (readlen == 0)
    throw exit_success ("EOF from server");
  response_len += readlen;

  for (;;)
    {
      if (response_len < 2)
        break;

      unsigned int cmd_len = response[0] << 8 | response[1];

      //LOG ("net") << "command length is ready: " << cmd_len << "\n";
      if (response_len < cmd_len + 2)
        break;

      //LOG ("net") << "command is ready; processing\n";
      on->receive (std::string (response + 2, response + 2 + cmd_len));
      memmove (response, response + 2 + cmd_len, response_len - 2 - cmd_len);
      response_len -= 2 + cmd_len;
      //LOG ("net") << response_len << " bytes left\n";
    }
}

void
communicator::send (std::string const &command)
{
  LOG ("net") << "sending command: " << command << "\n";
  write (sock.fd, &static_cast<uint16_t const &> (htons (command.length ())), 2);
  if (write (sock.fd, command.data (), command.length ()) == -1)
    throw "write: " + strerror ();
}
