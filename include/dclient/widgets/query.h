#pragma once

#include <ndk/frame.hh>

#include <vector>

struct query_widget
  : ndk::frame
{
  typedef std::vector<std::string> string_vector;

  explicit query_widget (panel *parent = 0);

  void update (string_vector &&query);

private:
  virtual void draw ();

  string_vector query;
};
