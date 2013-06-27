#include "dclient/widgets/stats.h"

#include "deliantra/data/counters.h"
#include "deliantra/data/map.h"
#include "deliantra/data/statistics.h"

#include <ndk/pen.hh>

stats_widget::stats_widget (ndk::panel *parent)
  : frame ("Statistics", parent)
  , stats (nullptr)
{
}

void
stats_widget::update (data::statistics const &stats)
{
  this->stats = &stats;
}

#include <sys/time.h>
#include <sys/resource.h>

static void
render_rusage (ndk::window &widget, int row = 8)
{
  static timeval start;

  ndk::pen pen (widget);
  return_unless (pen.move (2, row));

  if (!start.tv_sec)
    gettimeofday (&start, NULL);

  timeval now, diff;
  gettimeofday (&now, NULL);
  timersub (&now, &start, &diff);

  struct rusage usage;
  getrusage (RUSAGE_SELF, &usage);

  pen.write ( "[CPU: %ld.%02lds"
            , usage.ru_utime.tv_sec + usage.ru_stime.tv_sec
            , (usage.ru_utime.tv_usec + usage.ru_stime.tv_usec)
            / 10000
            );
  pen.write ( "   Cells: %d (%dKiB)"
            , data::cell_count
            , data::cell_count * sizeof (data::map::cell) / 1024
            );
  pen.write ( "   RES: %ldKiB"
            , usage.ru_maxrss
            );
  pen.write ( "   RT: %ld:%02ld]"
            , diff.tv_sec / 60
            , diff.tv_sec % 60
            );
}

void
stats_widget::draw ()
{
  frame::draw ();

  if (!stats)
    return;

  ndk::pen pen (*this);

  int const margin = 2;

  return_unless (pen.move (margin - 1, 1)); pen.write (stats->s_range);
  return_unless (pen.move (margin, 2)); pen.write (stats->s_title);
  return_unless (pen.move (margin, 3));
  if (stats->s_exp64)
    pen.write ("Exp: %'ld", stats->s_exp64);
  else if (stats->s_exp)
    pen.write ("Exp: %d", stats->s_exp);
  else
    pen.write ("Exp: 0");

  int row = 4;
  {
    int const shift = 12;
    int col = 0;

    return_unless (pen.move (margin + shift * col++, row)); pen.write ("HP: %d/%d", stats->s_hp, stats->s_maxhp);
    return_unless (pen.move (margin + shift * col++, row)); pen.write ("SP: %d/%d", stats->s_sp, stats->s_maxsp);
    return_unless (pen.move (margin + shift * col++, row)); pen.write ("GP: %d/%d", stats->s_grace, stats->s_maxgrace);
    return_unless (pen.move (margin + shift * col++, row)); pen.write ("Food: %d/%d", stats->s_food, 999);
  }
  row++;
  {
    int const shift = 12;
    int col = 0;

    return_unless (pen.move (margin + shift * col++, row)); pen.write ("WC: %d", stats->s_wc);
    return_unless (pen.move (margin + shift * col++, row)); pen.write ("AC: %d", stats->s_ac);
    return_unless (pen.move (margin + shift * col++, row)); pen.write ("Dam: %d", stats->s_dam);
    return_unless (pen.move (margin + shift * col++, row)); pen.write ("Speed: %d", stats->s_speed);
    return_unless (pen.move (margin + shift * col++, row)); pen.write ("Weapon speed: %d", stats->s_weap_sp);
  }
  row++;
  {
    int const shift = 8;
    int col = 0;

    return_unless (pen.move (margin + shift * col++, row)); pen.write ("Str: %d", stats->s_str);
    return_unless (pen.move (margin + shift * col++, row)); pen.write ("Dex: %d", stats->s_dex);
    return_unless (pen.move (margin + shift * col++, row)); pen.write ("Con: %d", stats->s_con);
    return_unless (pen.move (margin + shift * col++, row)); pen.write ("Int: %d", stats->s_int);
    return_unless (pen.move (margin + shift * col++, row)); pen.write ("Wis: %d", stats->s_wis);
    return_unless (pen.move (margin + shift * col++, row)); pen.write ("Pow: %d", stats->s_pow);
    return_unless (pen.move (margin + shift * col++, row)); pen.write ("Cha: %d", stats->s_cha);
  }

  render_rusage (*this);
}
