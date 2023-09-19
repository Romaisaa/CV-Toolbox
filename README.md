# Computer-Vision-Toolkit

## Table of contents:

- [Introduction](#introduction)
- [Full Project Features](#project-features)
- [Project Structure](#project-structure)
- [How to Run The Project](#run-the-project)
- [Team](#team)

### Introduction

Computer Vision Toolkit Application is a software solution designed to leverage the power of computer vision algorithms through an intuitive and user-friendly graphical user interface (GUI) developed using the Qt framework. This application aims to provide users with a versatile toolset for image analysis, making it suitable for a wide range of applications, including image processing, object detection,image features extraction, and more.

### Project Features:

This application mainly consists of 11 tabs (pages) to provide user with all needed CV algorithms, each page has certain features that are related to each other as follows:

1. Filters, Noise and Edge Detectors page:<br />
   Includes 3 types of noise that user can add to gray/coloured images:

   |         Uniform Noise          |         Guassian Noise          |        Salt & Pepper Noise         |
   | :----------------------------: | :-----------------------------: | :--------------------------------: |
   | ![](samples\uniform-noise.png) | ![](samples\guassian-noise.png) | ![](samples\salt-pepper-noise.png) |

   Includes 4 edge detectors types with the ability to control kernal size and any additional option for detector:
   | Sobel | Roberts | Perwitt | Canny |
   | :----------------------------: | :-----------------------------: | :--------------------------------: | :--------------------------------: |
   | ![](samples\sobel-detector.png) | ![](samples\roberts-detector.png) | ![](samples\perwitt-detector.png) | ![](samples\canny-detector.png) |

   Includes 3 types of filter enanling user to control kernal size:

   |         Average Filter          |         Guassian Filter          |         Median Filter          |
   | :-----------------------------: | :------------------------------: | :----------------------------: |
   | ![](samples\average-filter.png) | ![](samples\guassian-filter.png) | ![](samples\median-filter.png) |

   for more details about this page and results check [Report 1](docs\Report%201.pdf)

2. Normalization, Equalization and Thresholding page:<br />
   Shows images histogram with options to normalize or equalize:
   | Histogram | Normalization | Equalization |  
   | :----------------------------: | :-----------------------------: | :-----------------------------: |
   | ![](samples\histogram.png) | ![](samples\normalization.png) | ![](samples\equalizer.png) |

   Apply both global or local thresholding on uploaded image:
   | Local | Global |  
   | :----------------------------: | :-----------------------------: |
   | ![](samples\local-thresh.png) | ![](samples\global-thresh.png) |

   for more details about this page and results check [Report 1](docs\Report%201.pdf)

3. Frequency Domain Filters and Hybird Image:
   User upload 2 different images and determine the raduis for each filter type(high pass or low pass) and then can combine the output to get hybird image.
   ![](samples\hybird.png)
   for more details about this page and results check [Report 1](docs\Report%201.pdf)

4. Hough Transform Page:
   This page enbles user to apply Line/ Circle / Ellipse hough transform:
   | Line | Circle | Ellipse |  
   | :----------------------------: | :-----------------------------: | :-----------------------------: |
   | ![](samples\hough-line.png) | ![](samples\hough-circle.png) | ![](samples\hough-ellipse.png) |

   for more details about this page and results check [Report 2](docs\Report%202.pdf)

5. Active Contour Page:
   User has the ability to select cirlce fro image and tune paramters to contour objects.
   | Example 1 | Example 2 |  
   | :----------------------------: | :-----------------------------: |
   | ![](samples\active-contour.png) | ![](samples\active-contour-2.png) |

   for more details about this page and results check [Report 2](docs\Report%202.pdf)

6. Harris and Lambda Corner detector page:
   User can detect corners by harris or lambda methods and change kernal size and threshold.
   | Harris | Lambda |  
   | :----------------------------: | :-----------------------------: |
   | ![](samples\harris.png) | ![](samples\lambda.png) |

   for more details about this page and results check [Report 3](docs\Report%203.pdf)

7. Image Matching Methods page:
   User can match between image template and full image with 2 methods:
   | Square Sum of Differences method | Cross Correlation Method |  
   | :----------------------------: | :-----------------------------: |
   | ![](samples\match-ssd.png) | ![](samples\match-ncc.png) |

   for more details about this page and results check [Report 3](docs\Report%203.pdf)

8. Sift Descriptors Page:
   User can upload one or two image to get SIFT Keypoints with options to tune, and then compute keypoints matching between images.
   <p align="center">
     <img src="samples\sift-match.png" />
   </p>

   for more details about this page and results check [Report 3](docs\Report%203.pdf)

9. Advanced Thresholding Page:
   Includes Global Thresholding with 2 methods:
   |Ostu Thresholding| Optimal Thresholding |  
   | :----------------------------: | :-----------------------------: |
   | ![](samples\ostu-thresh.png) | ![](samples\optimal-thres.png) |

   Includes Local Thresholding and Multi-level Thresholding:
   |Local Thresholding| Multi-level Thresholding |  
   | :----------------------------: | :-----------------------------: |
   | ![](samples\local-thresh-2.png) | ![](samples\multi-thresh.png) |

   for more details about this page and results check [Report 4](docs\Report%204.pdf)

10. Segmentation Methods Page:
    User can segment RGB images with 4 types of segmentation:
    | K-Means Segmentation | Mean Shift Segmentation | Agglomerative Segmentation | Region Growing |
    | :----------------------------: | :-----------------------------: | :--------------------------------: | :--------------------------------: |
    | ![](samples\k-means.png) | ![](samples\mean-shift-seg.png) | ![](samples\agg-seg.png) | ![](samples\region-growing.png) |

for more details about this page and results check [Report 4](docs\Report%204.pdf)

11. Face Recognition and detection page:

- User can upload person or multi persons images and detect faces in images.
- User can upload training folder includes images named with its label.
- Once training finishes, User can upload new image to predict person in the image
- if model already exists, Then user can work dirctley without training
- For new training, user can determin number of PCA compenets to train a nearest neigbour model.
- User can upload test folder to visulaize ROC for each label.
