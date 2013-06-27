#pragma once

#include <ndk/edit.hh>
#include <ndk/frame.hh>
#include <ndk/menu.hh>
#include <ndk/panel.hh>
#include <ndk/text.hh>

#include <boost/scoped_ptr.hpp>

struct inventory_widget;
struct map_widget;
struct query_widget;
struct stats_widget;

struct widgets
{
  boost::scoped_ptr<map_widget> map;
  boost::scoped_ptr<ndk::text> info;
  boost::scoped_ptr<stats_widget> stats;
  boost::scoped_ptr<ndk::edit> input;
  boost::scoped_ptr<inventory_widget> inventory;
  boost::scoped_ptr<inventory_widget> container;
  boost::scoped_ptr<ndk::menu> invmenu;

  boost::scoped_ptr<query_widget> query;
  boost::scoped_ptr<ndk::edit> query_input;

public:
  void repack ();
  void resize ();

  void maximise_current ();

  widgets ();
  ~widgets ();

  widgets (widgets const &rhs) = delete;
  widgets &operator = (widgets const &rhs) = delete;

  ndk::panel *maximised;
};
