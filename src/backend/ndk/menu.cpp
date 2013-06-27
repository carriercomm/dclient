#include <ndk/menu.hh>
#include <ndk/application.hh>

using ndk::menu;

menu::item::item (char const *name, char const *description)
  : item_ (new_item ("name", "description"))
{
}

menu::item::item ()
  : item_ (nullptr)
{
}

menu::item::item (item &&other)
  : item_ (other.item_)
{
  other.item_ = nullptr;
}

menu::item &
menu::item::operator = (item &&rhs)
{
  item_ = rhs.item_;
  rhs.item_ = nullptr;
  return *this;
}

menu::item::~item ()
{
  if (item_)
    free_item (item_);
}


menu::menu (panel *parent)
  : panel (parent)
  , menu_ (nullptr, free_menu)
{
#if 0
  events ()->on_keyboard (slot (&menu::at_keyboard, this));

  add_item ("name1", "description1");
  add_item ("name2", "description2");
  add_item ("name3", "description3");
  items_.emplace_back ();

  menu_.reset (new_menu ((ITEM **)&items_[0]));
  int rows, columns;
  scale_menu (menu_.get (), &rows, &columns);
  resize (columns, rows);
  move (130, 10);
  set_menu_sub (menu_.get (), win ());

  visualize ();
#endif
}

menu::~menu ()
{
}

void
menu::add_item (char const *name, char const *description)
{
  menu_ = nullptr;
  items_.emplace_back (name, description);
}

void
menu::draw ()
{
  panel::draw ();

  if (!menu_)
    return;

  unpost_menu (menu_.get ());
  post_menu (menu_.get ());
}

ndk::event::result
menu::at_keyboard (keyboard const &ev)
{
  event::result res;

  switch (ev.code ())
    {
    case key::up:
      menu_driver (menu_.get (), REQ_UP_ITEM);
      res = event::accepted;
      break;

    case key::down:
      menu_driver (menu_.get (), REQ_DOWN_ITEM);
      res = event::accepted;
      break;

    default:
      res = event::ignored;
      break;
    }

  doupdate ();
  //visualize ();
  //if (res == event::accepted)
    //refresh ();

  return res;
}
