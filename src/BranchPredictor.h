/*
 * The branch perdiction module with the following strategies
 *   Always Taken
 *   Always Not Taken
 *   Backward Taken, Forward Not Taken
 *   Branch Prediction Buffer with 2bit history information
 * 
 * Created by He, Hao on 2019-3-25
 */

#ifndef BRANCH_PREDICTOR_H
#define BRANCH_PREDICTOR_H

#include <cstdint>
#include <string>

const int PRED_BUF_SIZE = 4096;

class BranchPredictor {
public:
  enum Strategy {
    AT, // Always Taken
    NT, // Always Not Taken
    BTFNT, // Backward Taken, Forward Not Taken
    BPB, // Branch Prediction Buffer with 2bit history information
    CBP32, // correlated branch prediction
    CBP16, // correlated branch prediction
    CBP8, // correlated branch prediction
    CBP4, // correlated branch prediction
  } strategy;

  BranchPredictor();
  ~BranchPredictor();

  bool predict(uint32_t pc, uint32_t insttype, int64_t op1, int64_t op2,
               int64_t offset);

  // For Branch Prediction Buffer 
  void update(uint32_t pc, bool branch);

  std::string strategyName();
  
private:
  enum PredictorState {
    STRONG_TAKEN = 0, WEAK_TAKEN = 1,
    STRONG_NOT_TAKEN = 3, WEAK_NOT_TAKEN = 2,
  } predbuf[PRED_BUF_SIZE]; // initial state: WEAK_TAKEN
  uint32_t ghb32 = 0; // global branch history register(32 bit long)
  uint16_t ghb16 = 0; // global branch history register(16 bit long)
  uint8_t ghb8 = 0; // global branch history register(8 bit long)
  uint8_t ghb4 = 0; // global branch history register(4 bit long using & 0xF)
};

#endif