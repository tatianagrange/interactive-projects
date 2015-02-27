package com.tgrange.android.nfcgame.util;

import android.content.Context;
import android.content.SharedPreferences;
import android.preference.PreferenceManager;

public class Preferences {

    /* **********************
     * 		Attributes		*
     ************************/
    private static Preferences instance = null;
    private boolean image1 = false;
    private boolean image2 = false;
    private boolean image3 = false;
    private boolean image4 = false;
    private boolean image5 = false;
    private boolean image6 = false;

    /* **********************
     * 		Constructor		*
     * ********************** */
    private Preferences(){
    }

    public static Preferences getInstance(){
        if(instance == null)
            instance = new Preferences();
        return instance;
    }

    /* **********************
     * 		Accessors		*
     * ********************** */
    public void setImage1(boolean image1) {
        this.image1 = image1;
    }

    public void setImage2(boolean image2) {
        this.image2 = image2;
    }

    public void setImage3(boolean image3) {
        this.image3 = image3;
    }

    public void setImage4(boolean image4) {
        this.image4 = image4;
    }

    public void setImage5(boolean image5) {
        this.image5 = image5;
    }

    public void setImage6(boolean image6) {
        this.image6 = image6;
    }

    public boolean isImage1() {
        return image1;
    }

    public boolean isImage2() {
        return image2;
    }

    public boolean isImage3() {
        return image3;
    }

    public boolean isImage4() {
        return image4;
    }

    public boolean isImage5() {
        return image5;
    }

    public boolean isImage6() {
        return image6;
    }


	/* **************************
	 * 		Public method		*
	 * ************************** */


    /**
     * This method is to used each time we change the preferences of the application
     * @param c
     */
    public void savePref(Context c)
    {
        SharedPreferences settings = PreferenceManager.getDefaultSharedPreferences(c); // recupere les settings
        SharedPreferences.Editor edit = settings.edit(); // recupere l'editeur pour editer les settings

        edit.putBoolean("image1", image1);
        edit.putBoolean("image2", image2);
        edit.putBoolean("image3", image3);
        edit.putBoolean("image4", image4);
        edit.putBoolean("image5", image5);
        edit.putBoolean("image6", image6);
        edit.commit();

    }

    /**
     * This method is to call at the begining of the application to load all preferences
     * @param c
     */
    public void loadPref(Context c)
    {
        SharedPreferences settings = PreferenceManager.getDefaultSharedPreferences(c); // recupere les settings

        image1 = settings.getBoolean("image1", image1);
        image2 = settings.getBoolean("image2", image2);
        image3 = settings.getBoolean("image3", image3);
        image4 = settings.getBoolean("image4", image4);
        image5 = settings.getBoolean("image5", image5);
        image6 = settings.getBoolean("image6", image6);
    }

    // Indice: C'est ici qu'on sauvegarde les préférence. L'état de l'application.
    //
    // A votre place, je ferai un tour au lab...
}