package com.tgrange.android.nfcgame;

import android.os.Bundle;
import android.widget.ImageView;

import com.tgrange.android.nfcgame.util.NFCActivity;
import com.tgrange.android.nfcgame.util.Preferences;


public class MainActivity extends NFCActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        Preferences pref = Preferences.getInstance();
        pref.loadPref(this);

        if(pref.isImage1()){
            ((ImageView)findViewById(R.id.image1)).setImageResource(R.drawable.win);
        }
        if(pref.isImage1()){
            ((ImageView)findViewById(R.id.image2)).setImageResource(R.drawable.win);
        }
        if(pref.isImage2()){
            ((ImageView)findViewById(R.id.image3)).setImageResource(R.drawable.win);
        }
        if(pref.isImage3()){
            ((ImageView)findViewById(R.id.image4)).setImageResource(R.drawable.win);
        }
        if(pref.isImage4()){
            ((ImageView)findViewById(R.id.image5)).setImageResource(R.drawable.win);
        }
        if(pref.isImage5()){
            ((ImageView)findViewById(R.id.image6)).setImageResource(R.drawable.win);
        }
    }

    @Override
    public void onResult(String result) {

        if(result.equals("image1")){
            Preferences.getInstance().setImage1(true);
            ((ImageView)findViewById(R.id.image1)).setImageResource(R.drawable.win);
        }else if(result.equals("image2")){
            Preferences.getInstance().setImage2(true);
            ((ImageView)findViewById(R.id.image2)).setImageResource(R.drawable.win);
        }else if(result.equals("image3")){
            Preferences.getInstance().setImage3(true);
            ((ImageView)findViewById(R.id.image3)).setImageResource(R.drawable.win);
        }else if(result.equals("image4")){
            Preferences.getInstance().setImage4(true);
            ((ImageView)findViewById(R.id.image4)).setImageResource(R.drawable.win);
        }else if(result.equals("image5")){
            Preferences.getInstance().setImage5(true);
            ((ImageView)findViewById(R.id.image5)).setImageResource(R.drawable.win);
        }else if(result.equals("image6")){
            Preferences.getInstance().setImage6(true);
            ((ImageView)findViewById(R.id.image6)).setImageResource(R.drawable.win);
        }

        Preferences.getInstance().savePref(this);
    }

    @Override
    public void onPresentTag() {}
    @Override
    public void onEndWrite() {}
    @Override
    public void onError(Exception error) {}
}

// Indice: Vous n'espérez tout de même pas trouver des indices dans ce fichier? :)