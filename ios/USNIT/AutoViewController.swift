//
//  AutoViewController.swift
//  usnit_app
//
//  Created by dyno on 5/28/16.
//  Copyright © 2016 dyno. All rights reserved.
//

import Foundation
import UIKit
import GoogleMobileAds

class AutoViewController: UIViewController {
    
    @IBOutlet weak var viewSC: UIScrollView!
    @IBOutlet weak var viewInput: UIView!
    //@IBOutlet weak var viewUnit: UIView!
    var genViewInput:GBViewImp?
    var genViewUnit:GBViewImp?
    var m_ad:AdmobBanner!
    
    @IBOutlet weak var viewBanner: GADBannerView!
    
    override func viewDidLoad() {
        super.viewDidLoad()
               
        let res_path = NSBundle.mainBundle().pathForResource("ui", ofType: "json")
        if (res_path == nil){
            GBLogGen.instance()?.logerrf("ui.json not found \(#file) \(#function) \(#line)");
            return;
        }
        
        GBPlatformUtilityGen.instance()?.setPlatofrmExcutor(IOSExecutor())

        GBUiManagerGen.instance()?.initialize(res_path!, factory: GBViewFactoryImp.instance)
        genViewInput = GBViewImp(id:"input_view", view: viewInput, constroller: self)
        genViewUnit = GBViewImp(id:"unit_view", view: viewSC, constroller: self)

        GBUiManagerGen.instance()?.inject(genViewInput)
        GBUiManagerGen.instance()?.inject(genViewUnit)
        
        SFUsnitLogic.sharedInstance
        
        //*
        m_ad = AdmobBanner(bannerView: viewBanner,root: self)
        m_ad.didLoad("ca-app-pub-4953725946697554/8856468622")
        
        // set height of scview
        //let offsety = (self.tabBarController?.tabBar.frame.size.height)! + m_ad.m_bannerView.frame.size.height
       // view.addConstraint(NSLayoutConstraint(item: viewSC, attribute: .Bottom,
       //     relatedBy: .Equal, toItem: m_ad.bannerView , attribute: .Top, multiplier: 1, constant: 0))
        //*/
    }
    override func didReceiveMemoryWarning() {
        super.didReceiveMemoryWarning()
        // Dispose of any resources that can be recreated.
    }
    
    override func viewDidAppear(animated: Bool) {
        super.viewDidAppear(animated)
    }
    
    override func viewDidLayoutSubviews() {
        super.viewDidLayoutSubviews()
        
        var maxHeight : CGFloat = viewSC.contentSize.height
        if maxHeight < viewSC.frame.height{
            maxHeight = viewSC.frame.height
        }
        
        for subview in viewSC.subviews {
            let newHeight = subview.frame.origin.y + subview.frame.height
            if newHeight > maxHeight {
                maxHeight = newHeight
            }
        }
        //GBLogGen.instance()?.logerrf("old content height: \(viewSC.contentSize.height) frame height: \(viewUnit.frame.height)");
        // set content size
        viewSC.contentSize = CGSize(width: viewSC.contentSize.width, height: maxHeight)
        //GBLogGen.instance()?.logerrf("content height: \(maxHeight)");
        //GBLogGen.instance()?.logerrf("view rame:\(viewUnit.frame.origin.x) \(viewUnit.frame.origin.y) \(viewUnit.frame.width) \(maxHeight)");
    }

}
