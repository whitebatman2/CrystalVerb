//
//  FdnConfig.hpp
//  Miłosz Kolenderski
//

#ifndef FdnConfig_hpp
#define FdnConfig_hpp

namespace FdnConfig
{
    const int kFdnOrder = 8;
    const int kNumFeedbackFilterBands = 3;
    const int kNumFeedbackFilterSplits = kNumFeedbackFilterBands - 1;
    const int kFeedbackFilterOrder = 3;
    const int kNumOutputFilterBands = 5;
    const int kNumOutputFilterSplits = kNumOutputFilterBands - 1;
    const int kOutputFilterOrder = 3;
    const int kNumDiffuserStages = 4;
    const int kNumSliders = kNumFeedbackFilterBands * 2 - 1;
}

#endif /* FdnConfig_hpp */
