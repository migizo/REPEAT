/*
  ==============================================================================

    Repeat.h
    Created: 12 Mar 2021 5:52:55pm
    Author:  migizo

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "AnimatorByCount.h"
#include <cmath>
class Repeat {
public:
    void setInterval(int _interval) {
       if (preserveInterval != _interval) {
            if (this->interval == 0) {
                clear();
                enablePush = true;  // noneからinterval値が変わった時サンプリング開始する
            }
            preserveInterval = _interval;
            // interval変更時のfade処理の初期化
            animator.start(100, 1.0, 0.0);
        }
    }
    
    //------------------------------------------------------------------------
    void setParameter(double sampleRate, double bpm) {
        
        // バッファサイズ. bpm変化に対応したサイズと指定のbpmで使用するサイズ
        maxBufferSize = sampleRate * 60.0 / 1.0/*bpm*/ * 4.0; // bpm=1の時のsample数
        bufferSize = sampleRate * (60.0 / bpm * 4.0); // 1小節のsample数
    }
    
    //------------------------------------------------------------------------
    template <typename T>
    void process(T & val) {
//        val *= sin((double)count/(1000.0*(double)(interval+1)));
//        count++;
//        return;
        
        // バッファ書き込み
        if (enablePush) {
            // バッファをfullにするまでpushする
            if (buffer.size() < maxBufferSize) {
                buffer.push_back(val);
            }
            // バッファがfullになったらpushしないようにする
            if (buffer.size() == maxBufferSize) enablePush = false;
        }
        
        if (interval != 0) {
            // バッファ読み込み
            int divSize = (double)bufferSize / (double)interval;
            int divCount = count % divSize;
            const int fadeSize = 100;
            val = buffer[divCount];
            
            // 音の繋ぎ目をスムーズにする処理、プチ音対策(フェードアウト -> インの順番)
            // バッファの先頭で音量フェードイン
            if (divCount < fadeSize) {
                val *= juce::jmap((double)(divCount), 0.0, (double)(fadeSize - 1), 0.0, 1.0);
            }
            // バッファの後ろで音量フェードアウト
            else if (divCount > (divSize - fadeSize)) {
                val *= juce::jmap((double)(divCount), (double)(divSize - fadeSize), (double)(divSize - 1), 1.0, 0.0);
            }
        }
        
        // インターバル違いをスムーズに繋げる処理(フェードアウト -> インの順番)
        animator.update();
        if (animator.getIsProgress()) {
            double v = animator.getValue();
            val *= v;
            // fadeout complete -> update interval
            if (v <= 0.0 && animator.getNormalizedValue() >= 1.0) {
                animator.start(100, 0.0, 1.0);
                interval = preserveInterval;
            }
        }
        
        count++;
    }
    
    //------------------------------------------------------------------------
    void clear() {
        count = 0;
        buffer.clear();
    }
    
    //------------------------------------------------------------------------

private:
    int count = 0;
    std::vector<double> buffer;
    AnimatorByCount animator;
    
    int maxBufferSize;
    int bufferSize;
    bool enablePush = false;
    int interval = 0;
    int preserveInterval = 0;

};
