#include "dclient/renderer.h"
#include "dclient/render_glyph.h"
#include "dclient/widgets.h"
#include "dclient/widgets/map.h"
#include "dclient/widgets/stats.h"

#include "deliantra/data/map.h"
#include "deliantra/data/statistics.h"

#include <ndk/application.hh>
#include <ndk/color.hh>

renderer::renderer (widgets &ui)
  : ui (ui)
{
}

renderer::~renderer ()
{
}

void
renderer::drawinfo (int level, std::string const &info)
{
  ui.info->add_text (ndi_color (level & NDI_COLOR_MASK, 0), info + "\n");
  ui.info->visualize ();
}

void
renderer::map (data::map const &map)
{
  ui.map->update (map);
  ui.map->visualize ();
  ui.stats->visualize ();

  ndk::app->update ();
}

void
renderer::msg (int level, std::string const &channel, std::string const &msg)
{
  // check for special types indicated by level
  // TEMPORARY WORKAROUND //
  if (level == 255)
    return;
  // END //
  drawinfo (level, channel + ": " + msg);
}

void
renderer::stats (data::statistics const &stats)
{
  ui.stats->update (stats);
  ui.stats->visualize ();
}
