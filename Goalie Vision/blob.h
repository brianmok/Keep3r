//
//  blob.h
//  Goalie Vision
//
//  Created by David Saltzman on 5/15/13.
//  Copyright (c) 2013 David Saltzman. All rights reserved.
//

#ifndef Goalie_Vision_blob_h
#define Goalie_Vision_blob_h


int DetectBlobsNoStillImage();

int DetectBlobsShowStillImage();

int DetectBlobsNoVideo(int captureWidth, int captureHeight);

int DetectBlobsShowVideo(int captureWidth, int captureHeight);

#endif
