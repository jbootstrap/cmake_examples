#include <opencv2/opencv.hpp>
//#include <opencv2/opencv_lib.hpp>
#include <opencv2/highgui/highgui.hpp>

int main(void)
{
  cv::Mat src_img;
  src_img = cv::imread("E:\\@Projects\\cmake_examples\\opencv_sample\\img12.jpg", 1);
  printf("src_img.type()=%d\n", src_img.type());
  // 画像が読み込まれなかったらプログラム終了
  if(src_img.empty()) return -1;

  // 結果画像表示
  cv::namedWindow("Image", CV_WINDOW_AUTOSIZE|CV_WINDOW_FREERATIO);
  cv::imshow("Image", src_img);
  cv::waitKey(0);
}
