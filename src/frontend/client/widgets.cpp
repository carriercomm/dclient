#include "dclient/widgets.h"
#include "dclient/widgets/inventory.h"
#include "dclient/widgets/map.h"
#include "dclient/widgets/query.h"
#include "dclient/widgets/stats.h"

#include <ndk/application.hh>
#include <ndk/frame.hh>

#include <csignal>

// stats height
static int const SH = 10;

using ndk::app;

void
widgets::repack ()
{
  if (maximised)
    {
      maximised->move (4, 2, ndk::absolute);
      maximised->resize (app->width () - 8, app->height () - 4);
    }
  else
    {
      map->move (0, 0, ndk::absolute);
      map->resize (app->width () / 2, app->height () / 3 * 2);

      info->move (map->right_x (), 0, ndk::absolute);
      info->resize (app->width () - map->width (), map->height () - SH);

      stats->move (info->x (), info->bottom_y (), ndk::absolute);
      stats->resize (info->width (), SH);

      input->move (map->x () + 2, map->bottom_y () + 1, ndk::absolute);
      input->resize (map->width () + info->width () - 4);

      inventory->move (map->x (), input->bottom_y () + 1, ndk::absolute);
      inventory->resize (map->width (), app->height () - map->height () - 3);

      container->move (stats->x (), inventory->y (), ndk::absolute);
      container->resize (stats->width (), inventory->height ());

      query->move ( (app->width () - query->width ()) / 2
                  , (app->height () - query->height ()) / 2
                  , ndk::absolute
                  );

      query_input->resize (query->width () - 2);
      query_input->move (query->x () + 1, query->bottom_y () - 2, ndk::absolute);
    }
}

void
widgets::resize ()
{
  repack ();

  map->visualize ();
  info->visualize ();
  stats->visualize ();
  inventory->visualize ();
  container->visualize ();
}

widgets::widgets ()
  : map (new map_widget)
  , info (new ndk::text ("Info", nullptr, ndk::text::wrap))
  , stats (new stats_widget)
  , input (new ndk::edit)
  , inventory (new inventory_widget)
  , container (new inventory_widget)
  , invmenu (new ndk::menu)
  , query (new query_widget)
  , query_input (new ndk::edit ("", query.get ()))
  , maximised (nullptr)
{
  query->resize (10, 5);

  resize ();

  map->focus ();
  //invmenu->focus ();
}

widgets::~widgets ()
{
}
