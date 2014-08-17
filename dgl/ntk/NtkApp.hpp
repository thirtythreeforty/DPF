/*
 * DISTRHO Plugin Framework (DPF)
 * Copyright (C) 2012-2014 Filipe Coelho <falktx@falktx.com>
 *
 * Permission to use, copy, modify, and/or distribute this software for any purpose with
 * or without fee is hereby granted, provided that the above copyright notice and this
 * permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES WITH REGARD
 * TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS. IN
 * NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL
 * DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER
 * IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN
 * CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#ifndef DGL_NTK_APP_HPP_INCLUDED
#define DGL_NTK_APP_HPP_INCLUDED

#include "../Base.hpp"

#include <list>
#include <FL/Fl.H>
#include <FL/Fl_Double_Window.H>
#include <FL/Fl_Shared_Image.H>
#include <FL/x.H>

START_NAMESPACE_DGL

class NtkWindow;

// -----------------------------------------------------------------------

/**
   DGL compatible App class that uses NTK instead of OpenGL.
   @see App
 */
class NtkApp
{
public:
   /**
      Constructor.
    */
    NtkApp()
        : fIsRunning(false),
          fWindows()
    {
        static bool initialized = false;

        if (! initialized)
        {
            initialized = true;
            fl_register_images();
            fl_open_display();
        }
    }

   /**
      Destructor.
    */
    ~NtkApp()
    {
        DISTRHO_SAFE_ASSERT(! fIsRunning);

        fWindows.clear();
    }

   /**
      Idle function.
      This calls the NTK event-loop once.
    */
    void idle()
    {
        Fl::check();
    }

   /**
      Run the application event-loop until all Windows are closed.
      @note: This function is meant for standalones only, *never* call this from plugins.
    */
    void exec()
    {
        fIsRunning = true;
        Fl::run();
        fIsRunning = false;
    }

   /**
      Quit the application.
      This stops the event-loop and closes all Windows.
    */
    void quit()
    {
        fIsRunning = false;

        for (std::list<Fl_Double_Window*>::reverse_iterator rit = fWindows.rbegin(), rite = fWindows.rend(); rit != rite; ++rit)
        {
            Fl_Double_Window* const window(*rit);
            window->hide();
        }
    }

   /**
      Check if the application is about to quit.
      Returning true means there's no event-loop running at the moment.
    */
    bool isQuiting() const noexcept
    {
        return !fIsRunning;
    }

private:
    bool fIsRunning;
    std::list<Fl_Double_Window*> fWindows;

    friend class NtkWindow;

    DISTRHO_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(NtkApp)
};

// -----------------------------------------------------------------------

END_NAMESPACE_DGL

#endif // DGL_NTK_APP_HPP_INCLUDED
