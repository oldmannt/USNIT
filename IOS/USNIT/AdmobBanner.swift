//
//  AdmobBanner.swift
//  USNIT
//
//  Created by dyno on 5/11/16.
//  Copyright © 2016 dyno. All rights reserved.
//

import Foundation
import UIKit
import GoogleMobileAds


class AdmobBanner: NSObject, GADBannerViewDelegate{
    var m_bannerView: GADBannerView!
    var m_rootController: UIViewController!
    
    init(root: UIViewController) {
        
        m_rootController = root
        m_bannerView = GADBannerView(adSize: kGADAdSizeSmartBannerPortrait)
        m_bannerView.rootViewController = m_rootController
        m_rootController.view.addSubview(m_bannerView)
    }
    
    init(bannerView: GADBannerView,root: UIViewController) {
        m_rootController = root
        m_bannerView = bannerView
        m_bannerView.rootViewController = m_rootController
    }
    
    func didLoad(adid: String){
        m_bannerView.translatesAutoresizingMaskIntoConstraints = false
        m_bannerView.delegate = self
        m_bannerView.adUnitID = adid
        
        if nil != m_rootController{
            bannerAboveTab(m_rootController!)
        }
        
        if UIDevice.currentDevice().orientation == .Portrait {
            m_bannerView.adSize = kGADAdSizeSmartBannerPortrait
        } else {
            m_bannerView.adSize = kGADAdSizeSmartBannerLandscape
        }
        
        m_bannerView.loadRequest(GADRequest())

    }
    
    func bannerUnderTab(root: UIViewController){
        
        // don't work
        var content_frame = root.view.frame
        let banner_fram = m_bannerView.sizeThatFits(content_frame.size)
        
        content_frame.size.height -= banner_fram.height;
        m_bannerView.frame.origin.y = content_frame.size.height
        
        root.tabBarController?.tabBar.frame.origin.y -= banner_fram.height
    }
    
    func bannerAboveTab(root: UIViewController){
        //let offsety:CGFloat = (rootController.tabBarController?.tabBar.frame.height)!
        root.view.addConstraint(NSLayoutConstraint(item: m_bannerView, attribute: .Bottom,
            relatedBy: .Equal, toItem: root.bottomLayoutGuide , attribute: .Top, multiplier: 1, constant: 0))
        root.view.addConstraint(NSLayoutConstraint(item: m_bannerView, attribute: .CenterX,
            relatedBy: .Equal, toItem: root.view, attribute: .CenterX, multiplier: 1, constant: 0))
    }
    
    // Called when an ad request loaded an ad.
    func adViewDidReceiveAd(bannerView: GADBannerView!) {
        //print(#function)

    }
    
    // Called when an ad request failed.
    func adView(bannerView: GADBannerView!, didFailToReceiveAdWithError error: GADRequestError!) {
        //print("\(#function): \(error.localizedDescription)")
    }
    
    // Called just before presenting the user a full screen view, such as a browser, in response to
    // clicking on an ad.
    func adViewWillPresentScreen(bannerView: GADBannerView!) {
        //print(#function)
    }
    
    // Called just before dismissing a full screen view.
    func adViewWillDismissScreen(bannerView: GADBannerView!) {
        //print(#function)
    }
    
    // Called just after dismissing a full screen view.
    func adViewDidDismissScreen(bannerView: GADBannerView!) {
        //print(#function)
    }
    
    // Called just before the application will background or terminate because the user clicked on an
    // ad that will launch another application (such as the App Store).
    func adViewWillLeaveApplication(bannerView: GADBannerView!) {
        //print(#function)
    }
}