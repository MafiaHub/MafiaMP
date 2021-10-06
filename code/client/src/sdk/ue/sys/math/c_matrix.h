#pragma once

#include <stdio.h>
#include "c_vector.h"

namespace SDK {
    namespace ue::sys::math {
        class C_Quat;
        class C_Matrix {
          public:
            void print() const {
                printf("[");

                int counter = 0;
                for (size_t i = 0; i < 12; i++, counter++) {
                    printf("%f, ", mData[i]);
                    if (counter > 2) {
                        printf("\n");
                        counter = -1;
                    }
                }

                printf("]\n");
            }

            void Identity(void) {
                mData[0]  = 1.0f;
                mData[1]  = 0.0f;
                mData[2]  = 0.0f;
                mData[3]  = 0.0f;
                mData[4]  = 1.0f;
                mData[5]  = 0.0f;
                mData[6]  = 0.0f;
                mData[7]  = 0.0f;
                mData[8]  = 1.0f;
                mData[9]  = 0.0f;
                mData[10] = 0.0f;
                mData[11] = 0.0f;
            }

            void SetPos(const C_Vector &pos) {
                mData[3]  = pos.x;
                mData[7]  = pos.y;
                mData[11] = pos.z;
            }

            void AddPos(const C_Vector &pos) {
                mData[3] += pos.x;
                mData[7] += pos.y;
                mData[11] += pos.z;
            }

            C_Vector GetPos() {
                return {mData[3], mData[7], mData[11]};
            }

            C_Vector GetRight() {
                return {mData[4], mData[5], mData[6]};
            }

            void Zero() {
                memset(mData, 0, sizeof(float) * 16);
            }

            void SetDir(C_Vector const &arg1, C_Vector const &arg2);
            void SetDir(C_Vector const &arg);
            void SetDir3(C_Vector const &arg);

            void SetRot(C_Quat const &rot);
            void SetRotEuler(float pitch, float yaw, float roll);

          private:
            float mData[16];
        };
    }; // namespace ue::sys::math
} // namespace SDK
