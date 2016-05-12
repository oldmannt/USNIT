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
    var bannerView: GADBannerView!
    var rootController: UIViewController!
    
    init(root: UIViewController) {
        
        rootController = root
        bannerView = GADBannerView()
        bannerView.rootViewController = rootController
    }
    
    func didLoad(adid: String){
        bannerView.translatesAutoresizingMaskIntoConstraints = false
        bannerView.delegate = self
        bannerView.adUnitID = adid
        
        rootController.view.addSubview(bannerView)
        
        let offsety:CGFloat = (rootController.tabBarController?.tabBar.frame.height)!
        rootController.view.addConstraint(NSLayoutConstraint(item: bannerView, attribute: .Bottom,
            relatedBy: .Equal, toItem: rootController.view , attribute: .Bottom, multiplier: 1, constant: -offsety))
        rootController.view.addConstraint(NSLayoutConstraint(item: bannerView, attribute: .CenterX,
            relatedBy: .Equal, toItem: rootController.view, attribute: .CenterX, multiplier: 1, constant: 0))
        
        if UIDevice.currentDevice().orientation == .Portrait {
            bannerView.adSize = kGADAdSizeSmartBannerPortrait
        } else {
            bannerView.adSize = kGADAdSizeSmartBannerLandscape
        }
        
        bannerView.loadRequest(GADRequest())

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