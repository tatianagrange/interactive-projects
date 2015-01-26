package com.tgrange.android.nfcexample.util;

import android.app.Activity;
import android.app.PendingIntent;
import android.content.Intent;
import android.nfc.NdefMessage;
import android.nfc.NdefRecord;
import android.nfc.NfcAdapter;
import android.nfc.Tag;
import android.nfc.tech.Ndef;
import android.nfc.tech.NdefFormatable;
import android.os.AsyncTask;
import android.os.Bundle;
import android.view.WindowManager;

import com.tgrange.android.nfcexample.R;

import java.io.UnsupportedEncodingException;
import java.nio.charset.Charset;
import java.util.Arrays;
import java.util.Locale;

/**
 * Created by Tatiana Grange on 22/01/2015.
 * This is a very big Class which manage NFC Intent
 */
public abstract class NFCActivity extends UtilActivity implements NFCListener {

    /* **********************
     * 		Attributes		*
     * ********************** */
    public static final String MIME_TEXT_PLAIN = "text/plain";
    private NfcAdapter mNfcAdapter;
    private String textToWrite;
    private boolean writeMode = false;
    private Tag mytag;

    /* **********************
     * 		Override		*
     * ********************** */
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        System.gc();
        getWindow().addFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);

        mNfcAdapter = NfcAdapter.getDefaultAdapter(this);
        if (mNfcAdapter == null) {
            // Stop here, we definitely need NFC
            Tools.toast(this, getString(R.string.nfc_device_no_nfc));
            finish();
            return;
        }

        if (!mNfcAdapter.isEnabled()) {
            Tools.toast(this, getString(R.string.nfc_nfc_not_activate));
            finish();
            return;
        }

        Tools.toast(this, getString(R.string.nfc_compatible_device));
    }

    @Override
    protected void onResume() {
        super.onResume();
        /**
         * It's important, that the activity is in the foreground (resumed). Otherwise
         * an IllegalStateException is thrown.
         */
        if (mNfcAdapter != null)
            setupForegroundDispatch(this, mNfcAdapter);
    }

    @Override
    protected void onPause() {
        /**
         * Call this before onPause, otherwise an IllegalArgumentException is thrown as well.
         */
        if (mNfcAdapter != null)
            stopForegroundDispatch(this, mNfcAdapter);
        super.onPause();
    }

    @Override
    protected void onNewIntent(Intent intent) {
        /**
         * This method gets called, when a new Intent gets associated with the current activity instance.
         * Instead of creating a new activity, onNewIntent will be called. For more information have a look
         * at the documentation.
         *
         * In our case this method gets called, when the user attaches a Tag to the device.
         */

        if (writeMode) {
            mytag = intent.getParcelableExtra(NfcAdapter.EXTRA_TAG);
            onPresentTag();

            if(textToWrite != null) {
                writeTag();
            }
        } else {
            Tools.log(getString(R.string.nfc_start_detection));
            handleIntent(intent);
        }
    }

    /**
     * @param activity The corresponding {@link Activity} requesting the foreground dispatch.
     * @param adapter  The {@link NfcAdapter} used for the foreground dispatch.
     */
    public static void setupForegroundDispatch(final Activity activity, NfcAdapter adapter) {
        final Intent intent = new Intent(activity.getApplicationContext(), activity.getClass());
        intent.setFlags(Intent.FLAG_ACTIVITY_SINGLE_TOP);
        final PendingIntent pendingIntent = PendingIntent.getActivity(activity.getApplicationContext(), 0, intent, 0);
        adapter.enableForegroundDispatch(activity, pendingIntent, null, null);
    }

    /**
     * @param activity The corresponding {@link Activity} requesting to stop the foreground dispatch.
     * @param adapter  The {@link NfcAdapter} used for the foreground dispatch.
     */
    public static void stopForegroundDispatch(final Activity activity, NfcAdapter adapter) {
        adapter.disableForegroundDispatch(activity);
    }

	/* **********************
	 * 		Own methods		*
	 * ********************** */

    public void setWriteMode(boolean writeMode) {
        this.writeMode = writeMode;
    }

    public void setTextToWrite(String textToWrite) {
        this.textToWrite = textToWrite;
    }

    /**
     * This function is call when a Intent for NFC is catch
     *
     * @param intent
     */
    private void handleIntent(Intent intent) {
        String action = intent.getAction();
        if (NfcAdapter.ACTION_NDEF_DISCOVERED.equals(action)) {
            String type = intent.getType();
            if (MIME_TEXT_PLAIN.equals(type)) {
                Tag tag = intent.getParcelableExtra(NfcAdapter.EXTRA_TAG);
                new NdefReaderTask().execute(tag);
            } else {
                Tools.toast(this, getString(R.string.nfc_bad_mime_type) + type);
            }
        } else {
            Tools.toast(this, getString(R.string.nfc_warning_ndef_discovered));

        }
    }

    /**
     * AsyncTask to process the information in the tag.
     *
     * @author tatiana
     */
    private class NdefReaderTask extends AsyncTask<Tag, Void, String> {

        @Override
        protected String doInBackground(Tag... params) {

            (new StopNFCTask(this)).start();

            Tag tag = params[0];
            Ndef ndef = Ndef.get(tag);
            if (ndef == null) {
                // NDEF is not supported by this Tag.
                return null;
            }

            NdefMessage ndefMessage = ndef.getCachedNdefMessage();
            NdefRecord[] records = ndefMessage.getRecords();
            for (NdefRecord ndefRecord : records) {
                if (ndefRecord.getTnf() == NdefRecord.TNF_WELL_KNOWN && Arrays.equals(ndefRecord.getType(), NdefRecord.RTD_TEXT)) {
                    try {
                        return readText(ndefRecord);
                    } catch (UnsupportedEncodingException e) {
                        Tools.toast(NFCActivity.this, getString(R.string.nfc_unsupoprted_encoding));
                    }
                }
            }
            return null;
        }


        private String readText(NdefRecord record) throws UnsupportedEncodingException {

            byte[] payload = record.getPayload();

            String textEncoding = ((payload[0] & 128) == 0) ? "UTF-8" : "UTF-16";

            int languageCodeLength = payload[0] & 0063;

            return new String(payload, languageCodeLength + 1, payload.length - languageCodeLength - 1, textEncoding);
        }

        @Override
        protected void onPostExecute(String result) {
            onResult(result);
        }
    }

    /**
     * This class stop the reading after 2 seconds
     */
    private class StopNFCTask extends Thread {
        private NdefReaderTask task;

        public StopNFCTask(NdefReaderTask task) {
            this.task = task;
        }

        @Override
        public void run() {
            try {
                sleep(2000);
                task.cancel(true);
                mytag = null;
            } catch (InterruptedException e) {
                // TODO Auto-generated catch block
                e.printStackTrace();
            }
            super.run();
        }
    }

    /**
     * This method is used to write on tag
     * @return
     */
    private boolean writeTag() {
        NdefMessage message = getNdefMessage();

        try {
            // If the tag is already formatted, just write the message to it
            Ndef ndef = Ndef.get(mytag);
            if(ndef != null) {
                ndef.connect();

                if(!ndef.isWritable()) {
                    error(new Exception(getString(R.string.nfc_tag_read_only)));
                    return false;
                }

                int size = message.toByteArray().length;
                if(ndef.getMaxSize() < size) {
                    error(new Exception(getString(R.string.nfc_size)));
                    return false;
                }

                ndef.writeNdefMessage(message);
                endWrite();
                return true;
            } else {
                NdefFormatable format = NdefFormatable.get(mytag);
                if(format != null) {
                    format.connect();
                    format.format(message);
                    endWrite();
                    return true;
                } else {
                    error(new Exception(getString(R.string.nfc_not_ndef)));
                    return false;
                }
            }
        } catch(Exception e) {
            error(e);
        }

        return false;
    }

    /**
     * Method to call at the end of writing
     */
    private void endWrite() {
        Tools.toast(this, getString(R.string.nfc_win));
        onEndWrite();
        writeMode = false;
    }

    /**
     * Method to call
     * @param e
     */
    private void error(Exception e){
        Tools.toast(this,getString(R.string.nfc_lose) + e.getMessage());
        writeMode = false;
        onError(e);
    }

    private NdefMessage getNdefMessage() {
        byte[] langBytes = Locale.ENGLISH.getLanguage().getBytes(Charset.forName("US-ASCII"));
        byte[] textBytes = textToWrite.getBytes(Charset.forName("UTF-8"));
        char status = (char) (langBytes.length);
        byte[] data = new byte[1 + langBytes.length + textBytes.length];
        data[0] = (byte) status;
        System.arraycopy(langBytes, 0, data, 1, langBytes.length);
        System.arraycopy(textBytes, 0, data, 1 + langBytes.length, textBytes.length);
        NdefRecord relayRecord = new NdefRecord(NdefRecord.TNF_WELL_KNOWN,
                NdefRecord.RTD_TEXT,
                new byte[0],
                data);

        // Complete NDEF message with both records
        return new NdefMessage(new NdefRecord[] {relayRecord});
    }
}