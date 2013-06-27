#include <boost/lexical_cast.hpp>

using boost::lexical_cast;

void
command_parser::on_anim (std::istream &args)
{
  LOG ("server") << "anim (" << lexical_cast<std::string> (args.rdbuf ()).length () << ")\n";
}
