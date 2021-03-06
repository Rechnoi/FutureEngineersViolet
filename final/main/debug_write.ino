#if USE_BLUETOOTH
#include "BluetoothSerial.h"
BluetoothSerial SerialBT;
#define Serial SerialBT
#endif

                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                              // Initializing debug output
void debugWriteSetup() {
#if USE_BLUETOOTH
    Serial.begin("Violet");
#else
    Serial.begin(500000);
#endif
}

int ptr_buff = 0;

// Clearing the buffer
void buffClear() {
    ptr_buff = 0;
}

// Adding a point to the buffer
void addBuffPoint(const Point& a) {
    buff[ptr_buff++] = a.x;
    buff[ptr_buff++] = a.y;
}

// Adding a segment to the buffer
void addBuffSegment(const Segment& seg) {
    addBuffPoint(seg.A);
    addBuffPoint(seg.B);
}

// Adding a sequence of bits to the buffer indicating the end of sending
void addBuffEnd() {
    buff[ptr_buff++] = 1 << 14;
}

// Adding a separating sequence of bits to the buffer
void addBuffSep() {
    buff[ptr_buff++] = (1 << 14) - 1;
}

// Adding a type value to the buffer int16_t
void addBuffInt16_t(uint16_t x) {
    buff[ptr_buff++] = x;
}

// Buffer output
void sendBuff() {
    Serial.write((byte*)buff, ptr_buff << 1);
}

// Debugging output
void debugWrite() {
    buffClear();
    for (int i = 0; i < cnt_points; ++i) {
        addBuffPoint(points[i]);
    }
    addBuffSep();
    for (int i = 0; i < 4; ++i) {
        addBuffSegment(border[i]);
    }

    addBuffSep();
    if ((!border[side_move_forward].empty()) && ((!border[BACK].empty()) || (!border[FORWARD].empty()))) {
        for (int i = 0; i < cnt_signs; ++i) {
            addBuffPoint(conv_rect_coord(signs[i]));
        }
    }
    
    addBuffSep();
    for (int side = 0; side < 4; ++side) {
        addBuffInt16_t(border[side].line.distToCenter());
    }

    for (int i = 0; i < 2; ++i) {
        for (int j = 0; j < 3; ++j) {
            addBuffInt16_t(is_sign[i][j]);
        }
    }
    
    addBuffInt16_t(dir_rotate);
    addBuffInt16_t(cnt_rotates);
    addBuffInt16_t(target_distance);
    addBuffInt16_t(cnt_points);
    addBuffInt16_t(time_turnover);
    addBuffInt16_t(diff_turnover);

    for (int i = 0; i < 2; ++i) {
        for (int j = 0; j < 2; ++j) {
            addBuffInt16_t(is_rotate_sign[i][j]);
        }
    }
    addBuffEnd();
    sendBuff();
}
