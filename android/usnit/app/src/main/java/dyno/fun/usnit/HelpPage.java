package dyno.fun.usnit;

import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.webkit.WebView;

import java.util.Locale;

import dyno.fun.gearsbox.AdmodBanner;
import dyno.fun.gearsbox.utils;

//import dyno.fun.gearsbox.LangType;
//import dyno.fun.gearsbox.LanguageStoreGen;

public class HelpPage extends AppCompatActivity {
    private AdmodBanner m_admod;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        setContentView(R.layout.helpweb);
        WebView wv = (WebView) findViewById(R.id.helpweb);

        if (Locale.getDefault().equals(Locale.CHINA)){
            wv.loadUrl("file:///android_asset/help/cn.html");
        }
        else{
            wv.loadUrl("file:///android_asset/help/en.html");
        }

        m_admod = new AdmodBanner(this, findViewById(R.id.ad_view), "ca-app-pub-4953725946697554~9493861829");
        m_admod.loadRequest();
        utils.setCurActivity(this);
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        if (m_admod != null) {
            m_admod.onDestroy();
        }
        utils.clearActivity(this);
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
        utils.setCurActivity(this);
    }
}
