/**------------------------------------------------------------*
 * @file    transparent.cpp
 * @brief   ����PNG�̕\��
 *------------------------------------------------------------*/
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
 
#include <stdlib.h>
#include <stdio.h>

#include "cvwin.hpp"
 
using namespace std;
 
const char winName[]="transparent";
 
/**-----------------------------------------------------------*
 * @fn  DrawTransPinP
 * @brief   ���߉摜���d�˂ĕ`�悷��
 * @param[out] img_dst
 * @param[in ] transImg �O�i�摜�B�A���t�@�`�����l���t���ł��邱��(CV_8UC4)
 * @param[in ] baseImg  �w�i�摜�B�A���t�@�`�����l���s�v(CV_8UC3)
 *------------------------------------------------------------*/
 void DrawTransPinP(cv::Mat &img_dst, const cv::Mat transImg, const cv::Mat baseImg, vector<cv::Point2f> tgtPt)
{
    cv::Mat img_rgb, img_aaa, img_1ma;
    vector<cv::Mat>planes_rgba, planes_rgb, planes_aaa, planes_1ma;
    int maxVal = pow(2, 8*baseImg.elemSize1())-1;
 
    //���߉摜��RGBA, �w�i�摜��RGB�̂݋��e�B�r�b�g�[�x�������摜�̂݋��e
    if(transImg.data==NULL || baseImg.data==NULL || transImg.channels()<4 ||baseImg.channels()<3 || (transImg.elemSize1()!=baseImg.elemSize1()) )
    {
        img_dst = cv::Mat(100,100, CV_8UC3);
        img_dst = cv::Scalar::all(maxVal);
        return;
    }
 
    //�����o������W���w�肳��Ă��Ȃ��ꍇ�͔w�i�摜�̒����ɔz�u����
    if(tgtPt.size()<4)
    {
        //���W�w��(�w�i�摜�̒��S�ɕ\������j
        int ltx = (baseImg.cols - transImg.cols)/2;
        int lty = (baseImg.rows - transImg.rows)/2;
        int ww  = transImg.cols;
        int hh  = transImg.rows;
 
        tgtPt.push_back(cv::Point2f(ltx   , lty));
        tgtPt.push_back(cv::Point2f(ltx+ww, lty));
        tgtPt.push_back(cv::Point2f(ltx+ww, lty+hh));
        tgtPt.push_back(cv::Point2f(ltx   , lty+hh));
    }
 
    //�ό`�s����쐬
    vector<cv::Point2f>srcPt;
    srcPt.push_back( cv::Point2f(0, 0) );
    srcPt.push_back( cv::Point2f(transImg.cols-1, 0) );
    srcPt.push_back( cv::Point2f(transImg.cols-1, transImg.rows-1) );
    srcPt.push_back( cv::Point2f(0, transImg.rows-1) );
    cv::Mat mat = cv::getPerspectiveTransform(srcPt, tgtPt);
 
    //�o�͉摜�Ɠ������E�����̃A���t�@�t���摜���쐬
    cv::Mat alpha0(baseImg.rows, baseImg.cols, transImg.type() );
    alpha0 = cv::Scalar::all(0);
    cv::warpPerspective(transImg, alpha0, mat,alpha0.size(), cv::INTER_CUBIC, cv::BORDER_TRANSPARENT);
 
    //�`�����l���ɕ���
    cv::split(alpha0, planes_rgba);
 
    //RGBA�摜��RGB�ɕϊ�   
    planes_rgb.push_back(planes_rgba[0]);
    planes_rgb.push_back(planes_rgba[1]);
    planes_rgb.push_back(planes_rgba[2]);
    merge(planes_rgb, img_rgb);
 
    //RGBA�摜����A���t�@�`�����l�����o   
    planes_aaa.push_back(planes_rgba[3]);
    planes_aaa.push_back(planes_rgba[3]);
    planes_aaa.push_back(planes_rgba[3]);
    merge(planes_aaa, img_aaa);
 
    //�w�i�p�A���t�@�`�����l��   
    planes_1ma.push_back(maxVal-planes_rgba[3]);
    planes_1ma.push_back(maxVal-planes_rgba[3]);
    planes_1ma.push_back(maxVal-planes_rgba[3]);
    merge(planes_1ma, img_1ma);
 
    img_dst = img_rgb.mul(img_aaa, 1.0/(double)maxVal) + baseImg.mul(img_1ma, 1.0/(double)maxVal);
}
 
/**------------------------------------------------------------*
 * @fn          main
 * @brief       ���C���֐�
 *------------------------------------------------------------*/
int main(int argc, char** argv)
{
    cv::Mat transImg, baseImg, img_dst;

	HINSTANCE hInstance = ::GetModuleHandle(NULL);

	HICON hIcon = ExtractIcon(
		hInstance,                   // �C���X�^���X�̃n���h��
		L"C:\\Windows\\notepad.exe", // �t�@�C����
		0                            // �A�C�R���̃C���f�b�N�X�ԍ�
	);

	transImg = cvwin::IconToMat(hIcon);

	DestroyIcon(hIcon);

    //�摜�ǂݍ���
    //transImg   = cv::imread("a.png", cv::IMREAD_UNCHANGED);
    baseImg = cv::imread("../img12.jpg");
 
    if( (transImg.data==NULL) || (baseImg.data==NULL) )
    {
        printf("------------------------------\n");
        printf("image not exist\n");
        printf("------------------------------\n");
        return EXIT_FAILURE;
    }
    else
    {
        printf("------------------------------\n");
        printf("Press ANY key to quit\n");
        printf("------------------------------\n");
    }
 
    cv::namedWindow(winName); //�E�B���h�E����
 
    //���W�w��(�w�i�摜�̒��S�ɕ\������j
    int ltx = (baseImg.cols - transImg.cols)/2;
    int lty = (baseImg.rows - transImg.rows)/2;
    int ww  = transImg.cols;
    int hh  = transImg.rows;
    vector<cv::Point2f>tgtPt;
 
    tgtPt.push_back(cv::Point2f(ltx   , lty));
    tgtPt.push_back(cv::Point2f(ltx+ww, lty));
    tgtPt.push_back(cv::Point2f(ltx+ww, lty+hh));
    tgtPt.push_back(cv::Point2f(ltx   , lty+hh));
 
    DrawTransPinP(img_dst, transImg, baseImg, tgtPt)  ;
    cv::imshow(winName, img_dst);
    cv::waitKey(0); //�L�[�{�[�h����
 
    return EXIT_SUCCESS;
}
