// Filename: tinySDLGraphicsPipe.cxx
// Created by:  drose (24Apr08)
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

#include "pandabase.h"

#ifdef HAVE_SDL

#include "tinySDLGraphicsPipe.h"
#include "tinySDLGraphicsWindow.h"
#include "tinyGraphicsStateGuardian.h"
#include "config_tinydisplay.h"
#include "frameBufferProperties.h"

TypeHandle TinySDLGraphicsPipe::_type_handle;

////////////////////////////////////////////////////////////////////
//     Function: TinySDLGraphicsPipe::Constructor
//       Access: Public
//  Description: 
////////////////////////////////////////////////////////////////////
TinySDLGraphicsPipe::
TinySDLGraphicsPipe() {
  _is_valid = true;

  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    tinydisplay_cat.error()
      << "Cannot initialize SDL video.\n";
    _is_valid = false;
  }
}

////////////////////////////////////////////////////////////////////
//     Function: TinySDLGraphicsPipe::Destructor
//       Access: Public, Virtual
//  Description: 
////////////////////////////////////////////////////////////////////
TinySDLGraphicsPipe::
~TinySDLGraphicsPipe() {
  if (SDL_WasInit(SDL_INIT_VIDEO)) {
    SDL_QuitSubSystem(SDL_INIT_VIDEO);
  }

  SDL_Quit();
}

////////////////////////////////////////////////////////////////////
//     Function: TinySDLGraphicsPipe::get_interface_name
//       Access: Published, Virtual
//  Description: Returns the name of the rendering interface
//               associated with this GraphicsPipe.  This is used to
//               present to the user to allow him/her to choose
//               between several possible GraphicsPipes available on a
//               particular platform, so the name should be meaningful
//               and unique for a given platform.
////////////////////////////////////////////////////////////////////
string TinySDLGraphicsPipe::
get_interface_name() const {
  return "TinyPanda";
}

////////////////////////////////////////////////////////////////////
//     Function: TinySDLGraphicsPipe::pipe_constructor
//       Access: Public, Static
//  Description: This function is passed to the GraphicsPipeSelection
//               object to allow the user to make a default
//               TinySDLGraphicsPipe.
////////////////////////////////////////////////////////////////////
PT(GraphicsPipe) TinySDLGraphicsPipe::
pipe_constructor() {
  return new TinySDLGraphicsPipe;
}

////////////////////////////////////////////////////////////////////
//     Function: TinySDLGraphicsPipe::make_output
//       Access: Protected, Virtual
//  Description: Creates a new window on the pipe, if possible.
////////////////////////////////////////////////////////////////////
PT(GraphicsOutput) TinySDLGraphicsPipe::
make_output(const string &name,
            const FrameBufferProperties &fb_prop,
            const WindowProperties &win_prop,
            int flags,
            GraphicsEngine *engine,
            GraphicsStateGuardian *gsg,
            GraphicsOutput *host,
            int retry,
            bool &precertify) {
  if (!_is_valid) {
    return NULL;
  }

  TinyGraphicsStateGuardian *tinygsg = 0;
  if (gsg != 0) {
    DCAST_INTO_R(tinygsg, gsg, NULL);
  }

  // First thing to try: a TinySDLGraphicsWindow

  if (retry == 0) {
    if (((flags&BF_require_parasite)!=0)||
        ((flags&BF_refuse_window)!=0)||
        ((flags&BF_resizeable)!=0)||
        ((flags&BF_size_track_host)!=0)||
        ((flags&BF_rtt_cumulative)!=0)||
        ((flags&BF_can_bind_color)!=0)||
        ((flags&BF_can_bind_every)!=0)) {
      return NULL;
    }
    return new TinySDLGraphicsWindow(engine, this, name, fb_prop, win_prop,
                                     flags, gsg, host);
  }
  
  // Nothing else left to try.
  return NULL;
}

#endif  // HAVE_SDL
