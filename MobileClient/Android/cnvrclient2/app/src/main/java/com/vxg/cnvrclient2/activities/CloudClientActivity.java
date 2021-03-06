/*
 *
 * Copyright (c) 2016 VIDEO EXPERTS GROUP
 *
 */

package com.vxg.cnvrclient2.activities;

import com.google.android.gms.common.ConnectionResult;
import com.google.android.gms.common.GoogleApiAvailability;
import com.vxg.cloud.platfrom.client.api.VXGCloudPlatform;
import com.vxg.cnvrclient2.ApplicationController;
import com.vxg.cnvrclient2.ApplicationMobileInterface;
import com.vxg.cnvrclient2.PlayerWrapper;
import com.vxg.cnvrclient2.R;
import com.vxg.cnvrclient2.WebPlayerInterface;
import com.vxg.cnvrclient2.controllers.UserProfileController;
import com.vxg.cnvrclient2.gcm.VXGCCQuickstartPreferences;

import android.app.Activity;
import android.content.Context;
import android.content.Intent;
import android.content.pm.PackageManager.NameNotFoundException;
import android.graphics.Color;
import android.graphics.Movie;
import android.os.Bundle;
import android.util.Log;
import android.view.MenuItem;
import android.view.MotionEvent;
import android.view.View;
import android.view.View.OnTouchListener;
import android.view.WindowManager;
import android.view.inputmethod.InputMethodManager;
import android.webkit.WebChromeClient;
import android.webkit.WebView;
import android.webkit.WebViewClient;
import android.widget.FrameLayout;
import android.widget.RelativeLayout;

import java.io.InputStream;

public class CloudClientActivity extends Activity
{
	private final String TAG = "Activity";
	public RelativeLayout main_layout = null;
	private ApplicationMobileInterface appMobileInterface = null;
	public WebPlayerInterface webPlayerInterface = null;
	private ApplicationController controller = ApplicationController.getInstance();

	/*@Override
	public boolean onOptionsItemSelected(MenuItem item) {
	    switch (item.getItemId()) {
	        case android.R.id.home:
	            onBackPressed();
	            return true;
	    }
	    return false;
	}*/
	
	@Override
	public void onBackPressed() {
		// Toast.makeText(getApplicationContext(),"Back button clicked", Toast.LENGTH_SHORT).show();
		if(appMobileInterface != null){
			appMobileInterface.execCallbackBack();
		}else{
			super.onBackPressed();
		}
	}

    private Movie mMovie;

    @Override
    public void onCreate(Bundle savedInstanceState) 
	{
		setTitle(R.string.app_name);
		super.onCreate(savedInstanceState);

        // TODO
		// Log.i(TAG, "GCM: open main activity " + getIntent().getStringExtra("gcmmessage"));
        // controller.setGcmLastMessage(getIntent().getStringExtra("gcmmessage"));
		// controller.initGCM(this);

		setContentView(R.layout.main);

        ((FrameLayout) findViewById(R.id.fl_cloudclient_loader)).setVisibility(View.VISIBLE);

        InputStream is = this.getResources().openRawResource(R.drawable.loader_white_330x28);
        mMovie = Movie.decodeStream(is);

		appMobileInterface = new ApplicationMobileInterface(this);
		webPlayerInterface = new WebPlayerInterface(this);
		
        main_layout = (RelativeLayout) findViewById(R.id.main_view);
		// main_layout.setBackgroundColor(Color.parseColor("#131313"));

        final int sdk = android.os.Build.VERSION.SDK_INT;
        if(sdk < android.os.Build.VERSION_CODES.JELLY_BEAN) {
            main_layout.setBackgroundDrawable( getResources().getDrawable(R.drawable.bg_cnvrclient2) );
        } else {
            main_layout.setBackground( getResources().getDrawable(R.drawable.bg_cnvrclient2) );
        }

        main_layout.setOnTouchListener(new OnTouchListener(){
			public boolean onTouch(View v, MotionEvent event) 
			{
				InputMethodManager inputManager = (InputMethodManager) getSystemService(Context.INPUT_METHOD_SERVICE);
				inputManager.hideSoftInputFromWindow(getWindow().getCurrentFocus().getWindowToken(), 0);
				return true;
			}
		});
        webPlayerInterface.playerLive = new PlayerWrapper(this, "live-container", (FrameLayout)findViewById(R.id.playerViewLive));
		webPlayerInterface.playerPlayback1 = new PlayerWrapper(this, "record-container1", (FrameLayout)findViewById(R.id.playerViewPlayback1));
		webPlayerInterface.playerPlayback2 = new PlayerWrapper(this, "record-container2", (FrameLayout)findViewById(R.id.playerViewPlayback2));

		WebView mWebView2 = (WebView) findViewById(R.id.webView2);
		mWebView2.setWebChromeClient(new WebChromeClient(){
			public void onConsoleMessage(String message, int lineNumber, String sourceID) {
			    Log.v("Test", message + " -- From line "
			                         + lineNumber + " of "
			                         + sourceID);
			}
		});

		mWebView2.setWebViewClient(new WebViewClient() {
			public void onPageFinished(WebView view, String url) {
				Log.v("Test", "Page loaded");
				// disable 10 minutes
				// view.loadUrl("javascript:setInterval(function(){ try { ifvisible.off('idle'); } catch(e) { console.log('idle off - error'); } }, 2000);");
			}
		});

        mWebView2.getSettings().setJavaScriptEnabled(true);
        mWebView2.getSettings().setBuiltInZoomControls(true);
        mWebView2.getSettings().setDomStorageEnabled(true);
        String versionName = "";
        try {
			versionName = getPackageManager().getPackageInfo(getPackageName(), 0).versionName;
		} catch (NameNotFoundException e) {
			e.printStackTrace();
		}

        String token = VXGCloudPlatform.inst().getCloudApiTokenID();
        String expire = VXGCloudPlatform.inst().getCloudApiTokenExpire();
		String baseuri = VXGCloudPlatform.inst().getBaseURI();
        // TODO add version
        String uri = baseuri + "/web/3.02/?mobile&vendor=VXG&login=test#token=" + token + "&expire=" + expire;

		mWebView2.loadUrl(uri);
		mWebView2.addJavascriptInterface(webPlayerInterface, "AndroidWebPlayerInterface");
		mWebView2.addJavascriptInterface(appMobileInterface, "ApplicationMobileInterface");
		mWebView2.clearCache(false);
		mWebView2.setBackgroundColor(Color.TRANSPARENT);
		this.getWindow().setSoftInputMode(WindowManager.LayoutParams.SOFT_INPUT_STATE_ALWAYS_HIDDEN);
    }

  	@Override
	protected void onStart() 
  	{
      	Log.e("SDL", "SDL onStart()");
		super.onStart();
	}

  	@Override
	protected void onStop() 
  	{
  		Log.e("SDL", "SDL onStop()");
		super.onStop();
		/*if (player_live != null)
			player_live.onStop();*/
	}

  	@Override
  	public void onWindowFocusChanged(boolean hasFocus) 
  	{
  		Log.e("SDL", "SDL onWindowFocusChanged(): " + hasFocus);
  		super.onWindowFocusChanged(hasFocus);
  		/*if (player_live != null)
			player_live.onWindowFocusChanged(hasFocus);*/
  	}

	@Override
	protected void onResume()
	{
		Log.e("SDL", "SDL onResume()");
		super.onResume();
		/*if (player_live != null)
			player_live.onResume();*/
		controller.registerReceiver(this);
	}

  	@Override
  	protected void onPause() {
		controller.unregisterReceiver(this);
  	    super.onPause();
  	}
  	
    
  	@Override
  	public void onLowMemory() 
  	{
  		Log.e("SDL", "SDL onLowMemory()");
  		super.onLowMemory();
  		/*if (player_live != null)
			player_live.onLowMemory();*/
  	}

  	@Override
  	protected void onDestroy() 
  	{
  		Log.e("SDL", "SDL onDestroy()");
		/*if (player_live != null)
			player_live.onDestroy();*/

		super.onDestroy();
   	}

    public void showUserProfile(){
        Intent intent = new Intent(CloudClientActivity.this, UserProfileActivity.class);
        startActivity(intent);
    }
}
