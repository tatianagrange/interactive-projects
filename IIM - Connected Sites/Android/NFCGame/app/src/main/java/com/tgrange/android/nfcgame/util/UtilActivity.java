package com.tgrange.android.nfcgame.util;

import android.app.Activity;
import android.os.Bundle;
import android.view.MotionEvent;
import android.view.View;
import android.view.inputmethod.InputMethodManager;

/**
 * Created by Tatiana Grange on 22/01/2015.
 */
public class UtilActivity extends Activity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
    }

    @Override
    public boolean onTouchEvent(MotionEvent event) {
        InputMethodManager inputMethodManager = (InputMethodManager) getSystemService(Activity.INPUT_METHOD_SERVICE);
        View currentFocus = this.getCurrentFocus();
        if(currentFocus != null)
            inputMethodManager.hideSoftInputFromWindow(currentFocus.getWindowToken(), 0);
        return true;
    }

    // Indice:
    //
    // Faîtes un tour du côté de chez ceux qui gèrent votre réseau
    // Mais ne leurs demandez rien! Les pauvres se font harceler!
    // C'est simplement bien caché dans le couloir. Donc laissez les en paix :)
}
