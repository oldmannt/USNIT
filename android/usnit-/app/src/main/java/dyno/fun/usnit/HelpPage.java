package dyno.fun.usnit;

import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.webkit.WebView;

import java.util.Locale;

import dyno.fun.gearsbox.LangType;
import dyno.fun.gearsbox.LanguageStoreGen;

public class HelpPage extends AppCompatActivity {

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

    }
}
