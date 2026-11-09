package io.github.jakerrw.crashbash;

import org.libsdl.app.SDLActivity;

/* The port's activity: SDL's own (its Java side, from SDL's source), running the game's main in libmain.so with the arguments an
 * intent's "args" extra gives, as a test run's are (docs/PORT.md, Android):
 *   adb shell am start -n io.github.jakerrw.crashbash/.CrashBashActivity --esa args --mode,1,--frames,600 */
public class CrashBashActivity extends SDLActivity {
    @Override
    protected String getMainFunction() {
        return "main";
    }

    @Override
    protected String[] getArguments() {
        String[] args = getIntent() != null ? getIntent().getStringArrayExtra("args") : null;
        return args != null ? args : new String[0];
    }
}
