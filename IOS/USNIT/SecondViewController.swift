//
//  SecondViewController.swift
//  USNIT
//
//  Created by dyno on 3/26/16.
//  Copyright © 2016 dyno. All rights reserved.
//

import UIKit

class SecondViewController: UIViewController {

    @IBOutlet var m_webHelp: UIWebView!
    var m_ad:AdmobBanner!
    
    override func viewDidLoad() {
        super.viewDidLoad()
        // Do any additional setup after loading the view, typically from a nib.
        
        let path = NSBundle.mainBundle().pathForResource("cn", ofType: "html")
        do {
            let fileHtml = try NSString(contentsOfFile: path!, encoding: NSUTF8StringEncoding)
            m_webHelp.loadHTMLString(fileHtml as String, baseURL: nil)
        }
        catch{
            
        }
        
        m_ad = AdmobBanner(root: self)
        m_ad.didLoad("ca-app-pub-4953725946697554/8856468622")
        
    }
    
    override func didReceiveMemoryWarning() {
        super.didReceiveMemoryWarning()
        // Dispose of any resources that can be recreated.
    }


}

