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
        
    }
    
    override func didReceiveMemoryWarning() {
        super.didReceiveMemoryWarning()
        // Dispose of any resources that can be recreated.
    }


}

