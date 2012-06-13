// Filename: cocoaGraphicsWindow.h
// Created by:  rdb (14May12)
//
////////////////////////////////////////////////////////////////////
//
// PANDA 3D SOFTWARE
// Copyright (c) Carnegie Mellon University.  All rights reserved.
//
// All use of this software is subject to the terms of the revised BSD
// license.  You should have received a copy of this license along
// with this source code in a file named "LICENSE."
//
////////////////////////////////////////////////////////////////////

#ifndef COCOAGRAPHICSWINDOW_H
#define COCOAGRAPHICSWINDOW_H

#include "pandabase.h"

#include "cocoaGraphicsPipe.h"
#include "graphicsWindow.h"

#import <AppKit/NSEvent.h>
#import <AppKit/NSView.h>
#import <AppKit/NSWindow.h>

////////////////////////////////////////////////////////////////////
//       Class : CocoaGraphicsWindow
// Description : An interface to the Cocoa system for managing
//               OpenGL windows under Mac OS X.
////////////////////////////////////////////////////////////////////
class CocoaGraphicsWindow : public GraphicsWindow {
public:
  CocoaGraphicsWindow(GraphicsEngine *engine, GraphicsPipe *pipe,
                      const string &name,
                      const FrameBufferProperties &fb_prop,
                      const WindowProperties &win_prop,
                      int flags,
                      GraphicsStateGuardian *gsg,
                      GraphicsOutput *host);
  virtual ~CocoaGraphicsWindow();

  virtual bool move_pointer(int device, int x, int y);
  virtual bool begin_frame(FrameMode mode, Thread *current_thread);
  virtual void end_frame(FrameMode mode, Thread *current_thread);
  virtual void end_flip();

  virtual void process_events();
  virtual void set_properties_now(WindowProperties &properties);

  void handle_move_event();
  void handle_resize_event();
  void handle_minimize_event(bool minimized);
  void handle_foreground_event(bool foreground);
  bool handle_close_request();
  void handle_close_event();
  void handle_key_event(NSEvent *event);
  void handle_mouse_button_event(int button, bool down);
  void handle_mouse_moved_event(bool in_window, int x, int y, bool absolute);
  void handle_wheel_event(double x, double y);

  INLINE NSWindow *get_nswindow() const;
  INLINE NSView *get_nsview() const;

protected:
  virtual void close_window();
  virtual bool open_window();

  CGDisplayModeRef find_display_mode(int width, int height);
  bool do_switch_fullscreen(CGDisplayModeRef mode);

  virtual void mouse_mode_absolute();
  virtual void mouse_mode_relative();

private:
  NSImage *load_image(const Filename &filename);

  ButtonHandle map_function_key(unsigned short keycode);

private:
  NSWindow *_window;
  NSView *_view;
  NSUInteger _modifier_keys;
  CGDirectDisplayID _display;
  CGDisplayModeRef _fullscreen_mode;
  CGDisplayModeRef _windowed_mode;
  bool _mouse_hidden;
  bool _context_needs_update;

  typedef pmap<Filename, NSImage*> IconImages;
  IconImages _images;

public:
  // Just so CocoaPandaView can access it.
  NSCursor *_cursor;

public:
  static TypeHandle get_class_type() {
    return _type_handle;
  }
  static void init_type() {
    GraphicsWindow::init_type();
    register_type(_type_handle, "CocoaGraphicsWindow",
                  GraphicsWindow::get_class_type());
  }
  virtual TypeHandle get_type() const {
    return get_class_type();
  }
  virtual TypeHandle force_init_type() {init_type(); return get_class_type();}

private:
  static TypeHandle _type_handle;
};

#include "cocoaGraphicsWindow.I"

#endif
