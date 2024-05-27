package com.PLP.Shapes_on_graph_paper;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;

import android.graphics.Color;

import android.content.pm.ActivityInfo;

//  Packages for Fullscreen

import androidx.core.view.WindowCompat;
import androidx.core.view.WindowInsetsControllerCompat;
import androidx.core.view.WindowInsetsCompat;



public class MainActivity extends AppCompatActivity
{
	@Override
	protected void onCreate(Bundle savedInstanceState)
	{
		
		
		super.onCreate(savedInstanceState);

		

		// Fullscreen

		// Get and Hide action bar
		getSupportActionBar().hide();

		// Do Fullscreen
		WindowCompat.setDecorFitsSystemWindows(getWindow(), false);

		// get window insets controller object.
		WindowInsetsControllerCompat windowInsetsControllerCompat = WindowCompat.getInsetsController(getWindow(),getWindow().getDecorView());

		// tell above object to hide system bars means status bar, naviation bar and caption bar and IME(input method editor).
		windowInsetsControllerCompat.hide(WindowInsetsCompat.Type.systemBars() | WindowInsetsCompat.Type.ime());

		// Do Landscape
		setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_LANDSCAPE);

		// Setting background color to black..
		getWindow().getDecorView().setBackgroundColor(Color.BLACK);

		GLESView glesView = new GLESView(this);
		
		
		setContentView(glesView);
	}

	@Override

	protected void onPause()
	{
		super.onPause();
	}

	@Override

	protected void onResume()
	{
		super.onResume();
	}
}

