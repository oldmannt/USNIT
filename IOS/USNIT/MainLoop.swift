//
//  MainLoop.swift
//  USNIT
//
//  Created by dyno on 5/3/16.
//  Copyright © 2016 dyno. All rights reserved.
//

import Foundation

import UIKit

class MainLoop : NSObject {
    
    var doSomething: (Int64) -> ()!
    var displayLink : CADisplayLink!
    var frameInterval : Int!
    var last : Double!
    
    init(frameInterval: Int, doSomething: (Int64) -> ()) {
        self.doSomething = doSomething
        self.frameInterval = frameInterval
        last = NSDate().timeIntervalSinceNow
        super.init()
        start()
    }
    
    // you could overwrite this too
    func handleTimer() {
        let now:Double = NSDate().timeIntervalSinceNow
        //print("handleTimer \((now - last)*1000)")
        
        doSomething(Int64((now - last)*1000))
        last = now
    }
    
    func start() {
        displayLink = CADisplayLink(target: self, selector: #selector(MainLoop.handleTimer))
        displayLink.frameInterval = frameInterval
        displayLink.addToRunLoop(NSRunLoop.mainRunLoop(), forMode: NSRunLoopCommonModes)
    }
    
    func stop() {
        displayLink.paused = true
        displayLink.removeFromRunLoop(NSRunLoop.mainRunLoop(), forMode: NSDefaultRunLoopMode)
        displayLink = nil
    }
}