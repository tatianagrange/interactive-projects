package com.tgrange.android.nfcexample;

import android.app.ProgressDialog;
import android.os.Bundle;
import android.view.View;
import android.widget.EditText;

import com.tgrange.android.nfcexample.util.NFCActivity;
import com.tgrange.android.nfcexample.util.Tools;


public class MainActivity extends NFCActivity implements View.OnClickListener {

    private ProgressDialog dialog;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        findViewById(R.id.btn).setOnClickListener(this);

    }

    @Override
    public void onResult(String result) {
        if(result != null)
            Tools.toast(this,"Sur le tag: " + result);
    }

    @Override
    public void onPresentTag() {
        Runnable changeMessage = new Runnable() {
            @Override
            public void run() {
                dialog.setMessage("Ecriture en cours");
            }
        };
        changeMessage.run();
    }

    @Override
    public void onEndWrite() {
        dialog.cancel();
    }

    @Override
    public void onClick(View v) {
        if(v.getId() == R.id.btn){
            setWriteMode(true);
            setTextToWrite(((EditText)findViewById(R.id.editText)).getText().toString());
            dialog = ProgressDialog.show(this, "","Approcher un tag", true);
        }
    }

    @Override
    public void onError(Exception error) {
        dialog.cancel();
    }
}
