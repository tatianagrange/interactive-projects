package com.tgrange.android.nfcgame.util;
public interface NFCListener {
    public void onResult(String result);
    public void onPresentTag();
    public void onEndWrite();
    public void onError(Exception error);
}
