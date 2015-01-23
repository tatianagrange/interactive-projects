package com.tgrange.android.nfcexample.util;

/**
 * Created by Tatiana Grange on 22/01/2015.
 */
public interface NFCListener {
    public void onResult(String result);
    public void onPresentTag();
    public void onEndWrite();
    public void onError(Exception error);
}
