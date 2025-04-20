/*
 * Created by He, Hao on 2019-3-25
 */

#include "BranchPredictor.h"
#include "Debug.h"

BranchPredictor::BranchPredictor() {
  for (int i = 0; i < PRED_BUF_SIZE; ++i) {
    this->predbuf[i] = WEAK_TAKEN;
  }
}

BranchPredictor::~BranchPredictor() {}

bool BranchPredictor::predict(uint32_t pc, uint32_t insttype, int64_t op1,
                              int64_t op2, int64_t offset) {
  switch (this->strategy) {
  case NT:
    return false;
  case AT:
    return true;
  case BTFNT: {
    if (offset >= 0) {
      return false;
    } else {
      return true;
    }
  }
  case BPB: {
    PredictorState state = this->predbuf[pc % PRED_BUF_SIZE];
    if (state == STRONG_TAKEN || state == WEAK_TAKEN) {
      return true;
    } else if (state == STRONG_NOT_TAKEN || state == WEAK_NOT_TAKEN) {
      return false;
    } else {
      dbgprintf("Strange Prediction Buffer!\n");
    }   
  }
  break;
  case CBP32: {
    PredictorState state = this->predbuf[(pc ^ this->ghb32) % PRED_BUF_SIZE];
    if (state == STRONG_TAKEN || state == WEAK_TAKEN) {
      return true;
    } else if (state == STRONG_NOT_TAKEN || state == WEAK_NOT_TAKEN) {
      return false;
    } else {
      dbgprintf("Strange Prediction Buffer!\n");
    }   
  }
  break;
  case CBP16: {
    PredictorState state = this->predbuf[(pc ^ this->ghb16) % PRED_BUF_SIZE];
    if (state == STRONG_TAKEN || state == WEAK_TAKEN) {
      return true;
    } else if (state == STRONG_NOT_TAKEN || state == WEAK_NOT_TAKEN) {
      return false;
    } else {
      dbgprintf("Strange Prediction Buffer!\n");
    }   
  }
  break;
  case CBP8: {
    PredictorState state = this->predbuf[(pc ^ this->ghb8) % PRED_BUF_SIZE];
    if (state == STRONG_TAKEN || state == WEAK_TAKEN) {
      return true;
    } else if (state == STRONG_NOT_TAKEN || state == WEAK_NOT_TAKEN) {
      return false;
    } else {
      dbgprintf("Strange Prediction Buffer!\n");
    }   
  }
  break;
  case CBP4: {
    PredictorState state = this->predbuf[(pc ^ this->ghb4) % PRED_BUF_SIZE];
    if (state == STRONG_TAKEN || state == WEAK_TAKEN) {
      return true;
    } else if (state == STRONG_NOT_TAKEN || state == WEAK_NOT_TAKEN) {
      return false;
    } else {
      dbgprintf("Strange Prediction Buffer!\n");
    }   
  }
  break;
  default:
    dbgprintf("Unknown Branch Perdiction Strategy!\n");
    break;
  }
  return false;
}

void BranchPredictor::update(uint32_t pc, bool branch) {
  int id = 0;
  switch(this->strategy) {
    case BPB: 
      id = pc % PRED_BUF_SIZE;
      break;
    case CBP32: 
      id = (pc ^ this->ghb32) % PRED_BUF_SIZE;
      break;
    case CBP16: 
      id = (pc ^ this->ghb16) % PRED_BUF_SIZE;
      break;
    case CBP8: 
      id = (pc ^ this->ghb8) % PRED_BUF_SIZE;
      break;
    case CBP4: 
      id = (pc ^ this->ghb4) % PRED_BUF_SIZE;
      break;
    default:
      break;
  }
  PredictorState state = this->predbuf[id];
  if (branch) {
    if (state == STRONG_NOT_TAKEN) {
      this->predbuf[id] = WEAK_NOT_TAKEN;
    } else if (state == WEAK_NOT_TAKEN) {
      this->predbuf[id] = WEAK_TAKEN;
    } else if (state == WEAK_TAKEN) {
      this->predbuf[id] = STRONG_TAKEN;
    } // do nothing if STRONG_TAKEN
    // update global history register
    this->ghb32 = (this->ghb32 << 1) + 1; 
    this->ghb16 = (this->ghb16 << 1) + 1; 
    this->ghb8 = (this->ghb8 << 1) + 1; 
    this->ghb4 = ((this->ghb4 << 1) + 1) & 0xF ; 
  } else { // not branch
    if (state == STRONG_TAKEN) {
      this->predbuf[id] = WEAK_TAKEN;
    } else if (state == WEAK_TAKEN) {
      this->predbuf[id] = WEAK_NOT_TAKEN;
    } else if (state == WEAK_NOT_TAKEN) {
      this->predbuf[id] = STRONG_NOT_TAKEN;
    } // do noting if STRONG_NOT_TAKEN
    this->ghb32 = this->ghb32 << 1;
    this->ghb16 = this->ghb16 << 1;
    this->ghb8 = this->ghb8 << 1;
    this->ghb4 = (this->ghb4 << 1) & 0xF ; 
  }
}

std::string BranchPredictor::strategyName() {
  switch (this->strategy) {
  case NT:
    return "Always Not Taken";
  case AT:
    return "Always Taken";
  case BTFNT:
    return "Back Taken Forward Not Taken";
  case BPB:
    return "Branch Prediction Buffer";
  case CBP32:
    return "Correlated Branch Prediction Buffer with 32 bit length of GHR";
  case CBP16:
    return "Correlated Branch Prediction Buffer with 16 bit length of GHR";
  case CBP8:
    return "Correlated Branch Prediction Buffer with 8 bit length of GHR";
  case CBP4:
    return "Correlated Branch Prediction Buffer with 4 bit length of GHR";
  default:
    dbgprintf("Unknown Branch Perdiction Strategy!\n");
    break;
  }
  return "error"; // should not go here
}