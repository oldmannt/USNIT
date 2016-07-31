/*
 * Copyright (C) 2013 Google, Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
package dyno.fun.usnit;

import android.content.Intent;
import android.content.res.Configuration;
import android.graphics.Color;
import android.graphics.Paint;
import android.graphics.drawable.GradientDrawable;
import android.graphics.drawable.ShapeDrawable;
import android.graphics.drawable.shapes.RectShape;
import android.os.Bundle;
import android.support.v7.app.ActionBarActivity;
import android.text.InputType;
import android.text.method.DigitsKeyListener;
import android.util.Log;
import android.view.View;
import android.view.Menu;
import android.view.MenuItem;
import android.view.ViewGroup;
import android.view.ViewTreeObserver;
import android.widget.EditText;
import android.widget.RelativeLayout;
import android.widget.TextView;
import android.widget.Toast;

import com.google.android.gms.common.GoogleApiAvailability;
import com.google.android.gms.common.GooglePlayServicesUtil;

import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.util.ArrayList;
import java.util.Locale;

import dyno.fun.gearsbox.AdmodBanner;
import dyno.fun.gearsbox.AsyncLoopGen;
import dyno.fun.gearsbox.KeyboardVisibilityEvent;
import dyno.fun.gearsbox.KeyboardVisibilityEventListener;
import dyno.fun.gearsbox.LangType;
import dyno.fun.gearsbox.LanguageStoreGen;
import dyno.fun.gearsbox.LogGen;
import dyno.fun.gearsbox.MainThreadLooper;
import dyno.fun.gearsbox.PlatformExcutorImp;
import dyno.fun.gearsbox.PlatformExecutorGen;
import dyno.fun.gearsbox.PlatformUtilityGen;
import dyno.fun.gearsbox.UiManagerGen;
import dyno.fun.gearsbox.UserConfigGen;
import dyno.fun.gearsbox.ViewFactoryImp;
import dyno.fun.gearsbox.ViewImp;
import dyno.fun.gearsbox.utils;
import dyno.fun.usnit.UilogicGen;

import static android.view.View.TEXT_ALIGNMENT_CENTER;

public class MainActivity extends ActionBarActivity {

    // Used to load the 'native-lib' library on application startup.
    static {
        System.loadLibrary("usnit_jni");
    }

    protected String getFilePath(String file_name){
        File file = new File(getFilesDir()+"/"+file_name);

        if (!file.exists()) try {
            InputStream is = getAssets().open(file_name);
            int size = is.available();
            byte[] buffer = new byte[size];
            is.read(buffer);
            is.close();

            FileOutputStream fos = new FileOutputStream(file);
            fos.write(buffer);
            fos.close();
        } catch (IOException e) {
            LogGen.instance().logerrf(String.format("getFilePath file:%s exception:%s", file_name, e.toString()));
            e.printStackTrace();
            return null;
        }

        return file.getAbsolutePath();
    }

    protected String getAssetsFileBuffer(String file_name){
        try {
            InputStream is = getAssets().open(file_name);
            int size = is.available();
            byte[] buffer = new byte[size];
            is.read(buffer);
            is.close();

            String rt = new String(buffer, "UTF-8");
            return  rt;
        } catch (IOException e) {
            LogGen.instance().logerrf(String.format("getAssetsFileBuffer file:%s exception:%s", file_name, e.toString()));
            e.printStackTrace();
            return null;
        }
    }

    protected String readFile(String file_name){
        String rt = getFilePath(file_name);
        if (rt != null){
            return rt;
        }

        Log.e("usnit", String.format("writer file error, read buffer. file name:%s", file_name));
        return getAssetsFileBuffer(file_name);
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        ViewGroup unit_view = (ViewGroup)findViewById(R.id.unite_scview);
        RelativeLayout.LayoutParams rlp = (RelativeLayout.LayoutParams)unit_view.getLayoutParams();

        m_admod = new AdmodBanner(this, findViewById(R.id.ad_view), "ca-app-pub-4953725946697554~9493861829");
        if (!m_admod.loadRequest()){
            //m_domob_banner = new DomobBanner(this, findViewById(R.id.domob), "56OJ2O0IuNyzr/fIYa", "16TLPmioApa3iNUU67WwA2_i");
            //if ((RelativeLayout.LayoutParams)unit_view.getLayoutParams()!=null){
            //    ((RelativeLayout.LayoutParams)unit_view.getLayoutParams()).addRule(RelativeLayout.ABOVE, R.id.domob);
            //}
        }else {
            if ((RelativeLayout.LayoutParams)unit_view.getLayoutParams()!=null){
                ((RelativeLayout.LayoutParams)unit_view.getLayoutParams()).addRule(RelativeLayout.ABOVE, R.id.ad_view);
            }
        }

        utils.setCurActivity(this);

        gearsbox();
        //test();
    }

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        // Inflate the menu; this adds items to the action bar if it is present.
        getMenuInflater().inflate(R.menu.menu_main, menu);
        return true;
    }

    @Override
    protected void onStart() {
        super.onStart();
        utils.setCurActivity(this);
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        if (m_admod != null) {
            m_admod.onDestroy();
        }
        utils.clearActivity(this);

        android.os.Process.killProcess(android.os.Process.myPid());
        System.exit(0);

    }

    @Override
    protected void onPause() {
        super.onPause();
        if (m_admod != null) {
            m_admod.onPause();
        }
        utils.clearActivity(this);
    }

    /** Called when returning to the activity */
    @Override
    protected void onResume() {
        super.onResume();
        if (m_admod != null) {
            m_admod.onResume();
        }
    }

    @Override
    protected void onStop() {
        super.onStop();
        utils.clearActivity(this);
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        // Handle action bar item clicks here. The action bar will
        // automatically handle clicks on the Home/Up button, so long
        // as you specify a parent activity in AndroidManifest.xml.
        int id = item.getItemId();

        //noinspection SimplifiableIfStatement
        if (id == R.id.action_settings) {
            PlatformUtilityGen.instance().getExcutor().endEniting(true);
            Intent intent = new Intent(MainActivity.this, HelpPage.class);
            startActivity(intent);
            return true;
        }

        return super.onOptionsItemSelected(item);
    }

    @Override
    public void onConfigurationChanged(Configuration newConfig) {
        super.onConfigurationChanged(newConfig);

        // Checks the orientation of the screen
        if (newConfig.orientation == Configuration.ORIENTATION_LANDSCAPE) {
            //Toast.makeText(this, "landscape", Toast.LENGTH_SHORT).show();
        } else if (newConfig.orientation == Configuration.ORIENTATION_PORTRAIT){
            //Toast.makeText(this, "portrait", Toast.LENGTH_SHORT).show();
        }

        //if (null != m_domob_banner){
        //    m_domob_banner.orientationChange();
        //}

        //test();
    }

    private void test(){
        GradientDrawable gd = new GradientDrawable();
        gd.setColor(0x0000FF00); // Changes this drawbale to use a single color instead of a gradient
        gd.setCornerRadius(0);
        gd.setStroke(1, 0xFFFF0000);

        ShapeDrawable sd = new ShapeDrawable();

        sd.setShape(new RectShape());
        sd.getPaint().setColor(Color.BLUE);
        sd.getPaint().setStrokeWidth(2);
        sd.getPaint().setStyle(Paint.Style.STROKE);

        ShapeDrawable sd2 = new ShapeDrawable();

        sd.setShape(new RectShape());
        sd.getPaint().setColor(Color.GREEN);
        sd.getPaint().setStrokeWidth(2);
        sd.getPaint().setStyle(Paint.Style.STROKE);

        //tv.setBackgroundDrawable(gd);

        RelativeLayout input_view = (RelativeLayout)findViewById(R.id.input_views);
        //input_view.setBackgroundDrawable(gd);
        input_view.setBackgroundColor(Color.argb(125, 0, 0, 255));
        int subviews = input_view.getChildCount();
        Log.d("test", "input_view child count:"+subviews);

        RelativeLayout.LayoutParams time_lp = new RelativeLayout.LayoutParams(
                ViewGroup.LayoutParams.WRAP_CONTENT, RelativeLayout.LayoutParams.WRAP_CONTENT);
        time_lp.addRule(RelativeLayout.ALIGN_TOP, input_view.getId());
        time_lp.addRule(RelativeLayout.ALIGN_LEFT, input_view.getId());
        time_lp.setMargins(100, time_lp.topMargin, time_lp.rightMargin, time_lp.bottomMargin);
        time_lp.setMargins(time_lp.leftMargin, 0, time_lp.rightMargin, time_lp.bottomMargin);

        TextView time_label = new TextView(this);
        time_label.setText("fire in the hole");
        time_label.setBackground(sd);
        time_label.setTextAlignment(TEXT_ALIGNMENT_CENTER);
        time_label.setId(View.generateViewId());
        time_label.setLayoutParams(time_lp);
        input_view.addView(time_label);


        RelativeLayout.LayoutParams input_lp = new RelativeLayout.LayoutParams(
                (int)(ViewGroup.LayoutParams.MATCH_PARENT), RelativeLayout.LayoutParams.WRAP_CONTENT);
        int time_id = time_label.getId();
        input_lp.addRule(RelativeLayout.BELOW, time_id);
        input_lp.addRule(RelativeLayout.CENTER_HORIZONTAL, RelativeLayout.TRUE);
        input_lp.setMargins(0,1,0,0);


        EditText input_edt = new EditText(this);
        input_edt.setBackground(sd2);
        input_edt.setInputType(InputType.TYPE_NUMBER_VARIATION_NORMAL);
        input_edt.setKeyListener(DigitsKeyListener.getInstance(false,true));
        input_edt.setLayoutParams(input_lp);
        input_edt.setId(R.id.input_unit);

        input_view.addView(input_edt);

        RelativeLayout unite_view = (RelativeLayout)findViewById(R.id.unite_views);
        unite_view.getViewTreeObserver().addOnGlobalLayoutListener(new ViewTreeObserver.OnGlobalLayoutListener() {
            @Override
            public void onGlobalLayout() {
                //Button btn1 = (Button)findViewById(R.id.button2);
                //Button btn2 = (Button)findViewById(R.id.button3);

                //btn2.getLayoutParams().width = btn1.getWidth();
            }
        });

        subviews = input_view.getChildCount();
        Log.d("test", "final input_view child count:"+subviews);

    }

    private void gearsbox(){
        m_looper = new MainThreadLooper(1000, new MainThreadLooper.Listenner(){

            @Override
            public void doSomething(long elapse) {
                AsyncLoopGen.instance().process(elapse);
            }
        });
        m_looper.start();
        PlatformUtilityGen.instance().setPlatofrmExcutor(new PlatformExcutorImp());
        String language_file = readFile("language.json");
        if (language_file==null){
            LogGen.instance().logerrf("get language.json failed");
            return;
        }

        if (!LanguageStoreGen.instance().initialize(language_file)){
            LogGen.instance().logerrf("get language initialize failed");
            return;
        }
        if (Locale.getDefault().equals(Locale.CHINA)){
            LanguageStoreGen.instance().setLanguage(LangType.LANG_CHS);
        }
        else{
            LanguageStoreGen.instance().setLanguage(LangType.LANG_ENG);
        }



        String ui_conf = readFile("ui.json");
        if (ui_conf==null){
            LogGen.instance().logerrf("get ui.json failed");
            return;
        }
        if (!UiManagerGen.instance().initialize(ui_conf, ViewFactoryImp.instance)){
            LogGen.instance().logerrf("initialize ui manager failed");
            return;
        }

        //input_view.setBackgroundColor(1,1,0,0);
        //unit_view.setBackgroundColor(1,0,1,0);

        String user_json = readFile("user.json");
        if (user_json==null){
            LogGen.instance().logerrf("get user.json failed");
            return;
        }
        UserConfigGen.instance().initialize(user_json);

        String uilgic_json = readFile("uilogic.json");
        if (uilgic_json==null){
            LogGen.instance().logerrf("get uilogic.json failed");
            return;
        }
        if (!UilogicGen.instance().initialize(uilgic_json)){
            LogGen.instance().logerrf("get uilogic initialize failed");
            return;
        }

        ArrayList<String> unit_names = UilogicGen.instance().getSubviews();
        for (String unit_name: unit_names){
            int id = getResources().getIdentifier(unit_name, "id", getPackageName());
            View view = findViewById(id);
            if (null == view){
                LogGen.instance().logerrf(String.format("add view_imp:%s, android view null", unit_name));
                continue;
            }
            ViewImp view_imp = new ViewImp(unit_name, view);
            UiManagerGen.instance().addView(view_imp);
            //LogGen.instance().logerrf(String.format("add view_imp:%s, viewimp:%s", unit_name, view_imp==null?"null":"ok"));
        }

        ViewImp input_view = new ViewImp("input_view", findViewById(R.id.input_views));
        ViewImp unit_view = new ViewImp("unit_view", findViewById(R.id.unite_views));
        UiManagerGen.instance().inject(input_view);
        UiManagerGen.instance().inject(unit_view);

        UilogicGen.instance().buildUi();

        EditText edit = (EditText)findViewById(R.id.input_text);
        edit.setOnFocusChangeListener(new View.OnFocusChangeListener() {
            @Override
            public void onFocusChange(View v, boolean hasFocus) {
                if(hasFocus){
                    //Toast.makeText(getApplicationContext(), "got the focus", Toast.LENGTH_LONG).show();
                }else {
                    //Toast.makeText(getApplicationContext(), "lost the focus", Toast.LENGTH_LONG).show();
                    PlatformUtilityGen.instance().getExcutor().endEniting(true);
                }
            }
        });
    }



    private MainThreadLooper m_looper;
    private AdmodBanner m_admod;
    //private DomobBanner m_domob_banner;
    /**
     * A native method that is implemented by the 'native-lib' native library,
     * which is packaged with this application.
     */

}
