package com.PLP.window;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;

import androidx.appcompat.widget.AppCompatTextView;

import android.graphics.Color;

import android.view.Gravity;

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

		// Setting background color to black..
		getWindow().getDecorView().setBackgroundColor(Color.BLACK);

		AppCompatTextView myTextView = new AppCompatTextView(this);
		myTextView.setTextColor(Color.rgb(0, 255, 0));
		myTextView.setTextSize(50);
		myTextView.setGravity(Gravity.CENTER);
		myTextView.setText("Hello World !!!");
		
		setContentView(myTextView);
	}
}
