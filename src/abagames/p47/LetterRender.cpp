/*
 * $Id:LetterRender.d,v 1.2 2004/01/01 11:26:43 kenta Exp $
 *
 * Copyright 2003 Kenta Cho. All rights reserved.
 */
/*
 * LetterRender.cpp 2012/04/24
 *
 * Copyright 2012 Koichi Yazawa. All rights reserved.
 */
#include <abagames/util/sdl/Screen3D.h>
#include <abagames/p47/LetterRender.h>
#include <abagames/p47/P47Screen.h>


int LetterRender::displayListIdx;
int LetterRender::colorIdx = 0;


void LetterRender::changeColor(int c) {
    colorIdx = c * LETTER_NUM;
}

void LetterRender::drawLetter(int n, float x, float y, float s, float d) {
    glPushMatrix();
    glTranslatef(x, y, 0);
    glScalef(s, s, s);
    glRotatef(d, 0, 0, 1);
    glCallList(displayListIdx + n + colorIdx);
    glPopMatrix();
}

void LetterRender::drawString(std::string str, float lx, float y, float s, int d) {
    float x = lx;
    int c;
    int idx;
    float ld;
    switch (d) {
    case TO_RIGHT:
        ld = 0;
        break;
    case TO_DOWN:
        ld = 90;
        break;
    case TO_LEFT:
        ld = 180;
        break;
    case TO_UP:
        ld = 270;
        break;
    }
    for (int i = 0; i < str.size(); i++) {
        c = str[i];
        if (c != ' ') {
            if (c >= '0' && c <= '9') {
                idx = c - '0';
            }
            else if (c >= 'A' && c <= 'Z') {
                idx = c - 'A' + 10;
            }
            else if (c >= 'a' && c <= 'z') {
                idx = c - 'a' + 10; 
            }
            else if (c == '.') {
                idx = 36;
            }
            else if (c == '-') {
                idx = 38;
            }
            else if (c == '+') {
                idx = 39;
            } else {
                idx = 37;
            }
            drawLetter(idx, x, y, s, ld);
        }
        switch(d) {
        case TO_RIGHT:
            x += s * 1.7f;
            break;
        case TO_DOWN:
            y += s * 1.7f;
            break;
        case TO_LEFT:
            x -= s * 1.7f;
            break;
        case TO_UP:
            y -= s * 1.7f;
            break;
        }
    }
}

void LetterRender::drawNum(int num, float lx, float y, float s, int d) {
    int n = num;
    float x = lx;
    float ld;
    switch (d) {
    case TO_RIGHT:
        ld = 0;
        break;
    case TO_DOWN:
        ld = 90;
        break;
    case TO_LEFT:
        ld = 180;
        break;
    case TO_UP:
        ld = 270;
        break;
    }
    for (;;) {
        drawLetter(n % 10, x, y, s, ld);
        switch(d) {
        case TO_RIGHT:
            x -= s * 1.7f;
            break;
        case TO_DOWN:
            y -= s * 1.7f;
            break;
        case TO_LEFT:
            x += s * 1.7f;
            break;
        case TO_UP:
            y += s * 1.7f;
            break;
        }
        n /= 10;
        if (n <= 0) break;
    }
}

void LetterRender::drawBox(float x, float y, float width, float height,
			   float r, float g, float b) {
    Screen3D::setColor(r, g, b, 0.5);
    P47Screen::drawBoxSolid(x - width, y - height, width * 2, height * 2);
    Screen3D::setColor(r, g, b, 1);
    P47Screen::drawBoxLine(x - width, y - height, width * 2, height * 2);
}

void LetterRender::drawLetter(int idx, float r, float g, float b) {
    float x, y, length, size, t;
    int deg;
    for (int i = 0;; i++) {
        deg = (int) spData[idx][i][4];
        if (deg > 99990) break;
        x = -spData[idx][i][0];
        y = -spData[idx][i][1];
        size = spData[idx][i][2];
        length = spData[idx][i][3];
        size *= 0.66f;
        length *= 0.6f;
        x = -x;
        y = y;
        deg %= 180;
        if (deg <= 45 || deg > 135)
            drawBox(x, y, size, length, r, g, b);
        else
            drawBox(x, y, length, size, r, g, b);
    }
}



void LetterRender::createDisplayLists() {
    displayListIdx = glGenLists(LETTER_NUM * 2);
    for (int i = 0; i < LETTER_NUM; i++) {
        glNewList(displayListIdx + i, GL_COMPILE);
        drawLetter(i, 1, 1, 1);
        glEndList();
    }
    for (int i = 0; i < LETTER_NUM; i++) {
        glNewList(displayListIdx + LETTER_NUM + i, GL_COMPILE);
        drawLetter(i, 1, 0.7, 0.7);
        glEndList();
    }
}

void LetterRender::deleteDisplayLists() {
    glDeleteLists(displayListIdx, LETTER_NUM * 2);
}

float LetterRender::spData[][16][5] =
{{
        {0, 1.15f, 0.65f, 0.3f, 0},
        {-0.6f, 0.55f, 0.65f, 0.3f, 90}, {0.6f, 0.55f, 0.65f, 0.3f, 90},
        {-0.6f, -0.55f, 0.65f, 0.3f, 90}, {0.6f, -0.55f, 0.65f, 0.3f, 90},
        {0, -1.15f, 0.65f, 0.3f, 0},
        {0, 0, 0, 0, 99999},
    },{
        {0, 0.55f, 0.65f, 0.3f, 90},
        {0, -0.55f, 0.65f, 0.3f, 90},
        {0, 0, 0, 0, 99999},
    },{
        {0, 1.15f, 0.65f, 0.3f, 0},
        {0.65f, 0.55f, 0.65f, 0.3f, 90},
        {0, 0, 0.65f, 0.3f, 0},
        {-0.65f, -0.55f, 0.65f, 0.3f, 90},
        {0, -1.15f, 0.65f, 0.3f, 0},
        {0, 0, 0, 0, 99999},
    },{
        {0, 1.15f, 0.65f, 0.3f, 0},
        {0.65f, 0.55f, 0.65f, 0.3f, 90},
        {0, 0, 0.65f, 0.3f, 0},
        {0.65f, -0.55f, 0.65f, 0.3f, 90},
        {0, -1.15f, 0.65f, 0.3f, 0},
        {0, 0, 0, 0, 99999},
    },{
        {-0.65f, 0.55f, 0.65f, 0.3f, 90}, {0.65f, 0.55f, 0.65f, 0.3f, 90},
        {0, 0, 0.65f, 0.3f, 0},
        {0.65f, -0.55f, 0.65f, 0.3f, 90},
        {0, 0, 0, 0, 99999},
    },{
        {0, 1.15f, 0.65f, 0.3f, 0},
        {-0.65f, 0.55f, 0.65f, 0.3f, 90},
        {0, 0, 0.65f, 0.3f, 0},
        {0.65f, -0.55f, 0.65f, 0.3f, 90},
        {0, -1.15f, 0.65f, 0.3f, 0},
        {0, 0, 0, 0, 99999},
    },{
        {0, 1.15f, 0.65f, 0.3f, 0},
        {-0.65f, 0.55f, 0.65f, 0.3f, 90},
        {0, 0, 0.65f, 0.3f, 0},
        {-0.65f, -0.55f, 0.65f, 0.3f, 90}, {0.65f, -0.55f, 0.65f, 0.3f, 90},
        {0, -1.15f, 0.65f, 0.3f, 0},
        {0, 0, 0, 0, 99999},
    },{
        {0, 1.15f, 0.65f, 0.3f, 0},
        {0.65f, 0.55f, 0.65f, 0.3f, 90},
        {0.65f, -0.55f, 0.65f, 0.3f, 90},
        {0, 0, 0, 0, 99999},
    },{
        {0, 1.15f, 0.65f, 0.3f, 0},
        {-0.65f, 0.55f, 0.65f, 0.3f, 90}, {0.65f, 0.55f, 0.65f, 0.3f, 90},
        {0, 0, 0.65f, 0.3f, 0},
        {-0.65f, -0.55f, 0.65f, 0.3f, 90}, {0.65f, -0.55f, 0.65f, 0.3f, 90},
        {0, -1.15f, 0.65f, 0.3f, 0},
        {0, 0, 0, 0, 99999},
    },{
        {0, 1.15f, 0.65f, 0.3f, 0},
        {-0.65f, 0.55f, 0.65f, 0.3f, 90}, {0.65f, 0.55f, 0.65f, 0.3f, 90},
        {0, 0, 0.65f, 0.3f, 0},
        {0.65f, -0.55f, 0.65f, 0.3f, 90},
        {0, -1.15f, 0.65f, 0.3f, 0},
        {0, 0, 0, 0, 99999},
    },{
        //A
        {0, 1.15f, 0.65f, 0.3f, 0},
        {-0.65f, 0.55f, 0.65f, 0.3f, 90}, {0.65f, 0.55f, 0.65f, 0.3f, 90},
        {0, 0, 0.65f, 0.3f, 0},
        {-0.65f, -0.55f, 0.65f, 0.3f, 90}, {0.65f, -0.55f, 0.65f, 0.3f, 90},
        {0, 0, 0, 0, 99999},
    },{
        {-0.1f, 1.15f, 0.45f, 0.3f, 0},
        {-0.65f, 0.55f, 0.65f, 0.3f, 90}, {0.45f, 0.55f, 0.65f, 0.3f, 90},
        {-0.1f, 0, 0.45f, 0.3f, 0},
        {-0.65f, -0.55f, 0.65f, 0.3f, 90}, {0.65f, -0.55f, 0.65f, 0.3f, 90},
        {0, -1.15f, 0.65f, 0.3f, 0},
        {0, 0, 0, 0, 99999},
    },{
        {0, 1.15f, 0.65f, 0.3f, 0},
        {-0.65f, 0.55f, 0.65f, 0.3f, 90},
        {-0.65f, -0.55f, 0.65f, 0.3f, 90},
        {0, -1.15f, 0.65f, 0.3f, 0},
        {0, 0, 0, 0, 99999},
    },{
        {-0.1f, 1.15f, 0.45f, 0.3f, 0},
        {-0.65f, 0.55f, 0.65f, 0.3f, 90}, {0.45f, 0.4f, 0.65f, 0.3f, 90},
        {-0.65f, -0.55f, 0.65f, 0.3f, 90}, {0.65f, -0.55f, 0.65f, 0.3f, 90},
        {0, -1.15f, 0.65f, 0.3f, 0},
        {0, 0, 0, 0, 99999},
    },{
        {0, 1.15f, 0.65f, 0.3f, 0},
        {-0.65f, 0.55f, 0.65f, 0.3f, 90},
        {0, 0, 0.65f, 0.3f, 0},
        {-0.65f, -0.55f, 0.65f, 0.3f, 90},
        {0, -1.15f, 0.65f, 0.3f, 0},
        {0, 0, 0, 0, 99999},
    },{// F
        {0, 1.15f, 0.65f, 0.3f, 0},
        {-0.65f, 0.55f, 0.65f, 0.3f, 90},
        {0, 0, 0.65f, 0.3f, 0},
        {-0.65f, -0.55f, 0.65f, 0.3f, 90},
        {0, 0, 0, 0, 99999},
    },{
        {0, 1.15f, 0.65f, 0.3f, 0},
        {-0.65f, 0.55f, 0.65f, 0.3f, 90},
        {0.25f, 0, 0.25f, 0.3f, 0},
        {-0.65f, -0.55f, 0.65f, 0.3f, 90}, {0.65f, -0.55f, 0.65f, 0.3f, 90},
        {0, -1.15f, 0.65f, 0.3f, 0},
        {0, 0, 0, 0, 99999},
    },{
        {-0.65f, 0.55f, 0.65f, 0.3f, 90}, {0.65f, 0.55f, 0.65f, 0.3f, 90},
        {0, 0, 0.65f, 0.3f, 0},
        {-0.65f, -0.55f, 0.65f, 0.3f, 90}, {0.65f, -0.55f, 0.65f, 0.3f, 90},
        {0, 0, 0, 0, 99999},
    },{
        {0, 0.55f, 0.65f, 0.3f, 90},
        {0, -0.55f, 0.65f, 0.3f, 90},
        {0, 0, 0, 0, 99999},
    },{
        {-0.65f, 0.55f, 0.65f, 0.3f, 90},
        {-0.65f, -0.55f, 0.65f, 0.3f, 90}, {0.65f, -0.75f, 0.25f, 0.3f, 90},
        {0, -1.15f, 0.65f, 0.3f, 0},
        {0, 0, 0, 0, 99999},
    },{//K
        {-0.65f, 0.55f, 0.65f, 0.3f, 90}, {0.45f, 0.55f, 0.65f, 0.3f, 90},
        {-0.1f, 0, 0.45f, 0.3f, 0},
        {-0.65f, -0.55f, 0.65f, 0.3f, 90}, {0.65f, -0.55f, 0.65f, 0.3f, 90},
        {0, 0, 0, 0, 99999},
    },{
        {-0.65f, 0.55f, 0.65f, 0.3f, 90},
        {-0.65f, -0.55f, 0.65f, 0.3f, 90},
        {0, -1.15f, 0.65f, 0.3f, 0},
        {0, 0, 0, 0, 99999},
    },{
        {-0.3f, 1.15f, 0.25f, 0.3f, 0}, {0.3f, 1.15f, 0.25f, 0.3f, 0},
        {-0.65f, 0.55f, 0.65f, 0.3f, 90}, {0.65f, 0.55f, 0.65f, 0.3f, 90},
        {-0.65f, -0.55f, 0.65f, 0.3f, 90}, {0.65f, -0.55f, 0.65f, 0.3f, 90},
        {0, 0.55f, 0.65f, 0.3f, 90},
        {0, -0.55f, 0.65f, 0.3f, 90},
        {0, 0, 0, 0, 99999},
    },{
        {0, 1.15f, 0.65f, 0.3f, 0},
        {-0.65f, 0.55f, 0.65f, 0.3f, 90}, {0.65f, 0.55f, 0.65f, 0.3f, 90},
        {-0.65f, -0.55f, 0.65f, 0.3f, 90}, {0.65f, -0.55f, 0.65f, 0.3f, 90},
        {0, 0, 0, 0, 99999},
    },{
        {0, 1.15f, 0.65f, 0.3f, 0},
        {-0.65f, 0.55f, 0.65f, 0.3f, 90}, {0.65f, 0.55f, 0.65f, 0.3f, 90},
        {-0.65f, -0.55f, 0.65f, 0.3f, 90}, {0.65f, -0.55f, 0.65f, 0.3f, 90},
        {0, -1.15f, 0.65f, 0.3f, 0},
        {0, 0, 0, 0, 99999},
    },{//P
        {0, 1.15f, 0.65f, 0.3f, 0},
        {-0.65f, 0.55f, 0.65f, 0.3f, 90}, {0.65f, 0.55f, 0.65f, 0.3f, 90},
        {0, 0, 0.65f, 0.3f, 0},
        {-0.65f, -0.55f, 0.65f, 0.3f, 90},
        {0, 0, 0, 0, 99999},
    },{
        {0, 1.15f, 0.65f, 0.3f, 0},
        {-0.65f, 0.55f, 0.65f, 0.3f, 90}, {0.65f, 0.55f, 0.65f, 0.3f, 90},
        {-0.65f, -0.55f, 0.65f, 0.3f, 90}, {0.65f, -0.55f, 0.65f, 0.3f, 90},
        {0, -1.15f, 0.65f, 0.3f, 0},
        {0.2f, -0.6f, 0.45f, 0.3f, 360-300},
        {0, 0, 0, 0, 99999},
    },{
        {0, 1.15f, 0.65f, 0.3f, 0},
        {-0.65f, 0.55f, 0.65f, 0.3f, 90}, {0.65f, 0.55f, 0.65f, 0.3f, 90},
        {-0.1f, 0, 0.45f, 0.3f, 0},
        {-0.65f, -0.55f, 0.65f, 0.3f, 90}, {0.45f, -0.55f, 0.65f, 0.3f, 90},
        {0, 0, 0, 0, 99999},
    },{
        {0, 1.15f, 0.65f, 0.3f, 0},
        {-0.65f, 0.55f, 0.65f, 0.3f, 90},
        {0, 0, 0.65f, 0.3f, 0},
        {0.65f, -0.55f, 0.65f, 0.3f, 90},
        {0, -1.15f, 0.65f, 0.3f, 0},
        {0, 0, 0, 0, 99999},
    },{
        {-0.4f, 1.15f, 0.45f, 0.3f, 0}, {0.4f, 1.15f, 0.45f, 0.3f, 0},
        {0, 0.55f, 0.65f, 0.3f, 90},
        {0, -0.55f, 0.65f, 0.3f, 90},
        {0, 0, 0, 0, 99999},
    },{//U
        {-0.65f, 0.55f, 0.65f, 0.3f, 90}, {0.65f, 0.55f, 0.65f, 0.3f, 90},
        {-0.65f, -0.55f, 0.65f, 0.3f, 90}, {0.65f, -0.55f, 0.65f, 0.3f, 90},
        {0, -1.15f, 0.65f, 0.3f, 0},
        {0, 0, 0, 0, 99999},
    },{
        {-0.65f, 0.55f, 0.65f, 0.3f, 90}, {0.65f, 0.55f, 0.65f, 0.3f, 90},
        {-0.5f, -0.55f, 0.65f, 0.3f, 90}, {0.5f, -0.55f, 0.65f, 0.3f, 90},
        {0, -1.15f, 0.45f, 0.3f, 0},
        {0, 0, 0, 0, 99999},
    },{
        {-0.65f, 0.55f, 0.65f, 0.3f, 90}, {0.65f, 0.55f, 0.65f, 0.3f, 90},
        {-0.65f, -0.55f, 0.65f, 0.3f, 90}, {0.65f, -0.55f, 0.65f, 0.3f, 90},
        {-0.3f, -1.15f, 0.25f, 0.3f, 0}, {0.3f, -1.15f, 0.25f, 0.3f, 0},
        {0, 0.55f, 0.65f, 0.3f, 90},
        {0, -0.55f, 0.65f, 0.3f, 90},
        {0, 0, 0, 0, 99999},
    },{
        {-0.4f, 0.6f, 0.85f, 0.3f, 360-120},
        {0.4f, 0.6f, 0.85f, 0.3f, 360-60},
        {-0.4f, -0.6f, 0.85f, 0.3f, 360-240},
        {0.4f, -0.6f, 0.85f, 0.3f, 360-300},
        {0, 0, 0, 0, 99999},
    },{
        {-0.4f, 0.6f, 0.85f, 0.3f, 360-120},
        {0.4f, 0.6f, 0.85f, 0.3f, 360-60},
        {0, -0.55f, 0.65f, 0.3f, 90},
        {0, 0, 0, 0, 99999},
    },{
        {0, 1.15f, 0.65f, 0.3f, 0},
        {0.35f, 0.5f, 0.65f, 0.3f, 360-60},
        {-0.35f, -0.5f, 0.65f, 0.3f, 360-240},
        {0, -1.15f, 0.65f, 0.3f, 0},
        {0, 0, 0, 0, 99999},
    },{// .
        {0, -1.15f, 0.05f, 0.3f, 0},
        {0, 0, 0, 0, 99999},
    },{// _
        {0, -1.15f, 0.65f, 0.3f, 0},
        {0, 0, 0, 0, 99999},
    },{// -
        {0, 0, 0.65f, 0.3f, 0},
        {0, 0, 0, 0, 99999},
    },{//+
        {-0.4f, 0, 0.45f, 0.3f, 0}, {0.4f, 0, 0.45f, 0.3f, 0},
        {0, 0.55f, 0.65f, 0.3f, 90},
        {0, -0.55f, 0.65f, 0.3f, 90},
        {0, 0, 0, 0, 99999},
    },{//'
        {0, 1.0f, 0.4f, 0.2f, 90},
        {0, 0, 0, 0, 99999},
    },{//''
        {-0.19f, 1.0f, 0.4f, 0.2f, 90},
        {0.2f, 1.0f, 0.4f, 0.2f, 90},
        {0, 0, 0, 0, 99999},
    }};

