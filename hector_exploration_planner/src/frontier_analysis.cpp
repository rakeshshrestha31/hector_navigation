//
// Created by rakesh on 14/05/18.
//

#include <hector_exploration_planner/frontier_analysis.h>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

namespace hector_exploration_planner
{
namespace frontier_analysis
{

void preprocessFrontierImg(cv::Mat &frontier_img_in, cv::Mat &frontier_img_out)
{
  cv::Mat element = cv::getStructuringElement( cv::MORPH_RECT,
                                               cv::Size( 2*FRONTIER_OPENING_KERNEL + 1, 2*FRONTIER_OPENING_KERNEL+1 ),
                                               cv::Point( FRONTIER_OPENING_KERNEL, FRONTIER_OPENING_KERNEL ) );

  cv::Mat temp_img = frontier_img_in.clone();

  // closing (hole filling)
  cv::dilate(frontier_img_in, temp_img, element);
  cv::erode(temp_img, frontier_img_out, element);

// opening (cleaning)
//  element = cv::getStructuringElement( cv::MORPH_RECT,
//                                       cv::Size( 2*FRONTIER_HOLE_FILLING_KERNEL + 1, 2*FRONTIER_HOLE_FILLING_KERNEL+1 ),
//                                       cv::Point( FRONTIER_HOLE_FILLING_KERNEL, FRONTIER_HOLE_FILLING_KERNEL ) );
//  cv::dilate(frontier_img_out, temp_img, element);
//  cv::erode(temp_img, frontier_img_out, element);

}
std::vector< std::vector<cv::Point> > groupFrontiers(cv::Mat &frontier_img,
                                                     std::vector<cv::Point> clustered_frontier_points)
{
//  for (const auto &frontier_point: clustered_frontier_points)
//  {
//    // TODO: use the clustered_frontier_points to floodfill (for efficiency and better frontiers)
//  }

  std::vector< std::vector<cv::Point> > contours;
  std::vector<cv::Vec4i> hierarchy;
  cv::findContours(frontier_img, contours, hierarchy, CV_RETR_LIST, CV_CHAIN_APPROX_NONE);
  return contours;
}

void colorFrontiers(cv::Mat &frontier_img,
                    std::vector< std::vector<cv::Point> > grouped_frontiers,
                    cv::RNG &rng,
                    cv::Mat &colored_frontier_img)
{
  colored_frontier_img = cv::Mat(frontier_img.size(), CV_8UC3);
  for (size_t i = 0; i < grouped_frontiers.size(); i++)
  {
    cv::Scalar color(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
    cv::drawContours(colored_frontier_img, grouped_frontiers, i, color, 2);
  }
}

} // namespace frontier_analysis
} // namespace hector_exploration_planner