#include <jni.h>
#include "opencv2/photo.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/core.hpp"
#include <android/log.h>

#define  LOG_TAG  "simplicity"

#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)
#define  LOGW(...)  __android_log_print(ANDROID_LOG_WARN,LOG_TAG,__VA_ARGS__)
#define  LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG,LOG_TAG,__VA_ARGS__)
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)

using namespace cv;
extern "C" {

// this deceleration signature was generated by the javah.exe tool from
JNIEXPORT jstring JNICALL Java_com_simplicity_maged_mccobjectdetection_services_NativeBlendInterface_blend(
		JNIEnv *jenv, jobject, jstring image1, jstring image2, jstring result,
		jint x, jint y);

JNIEXPORT jstring JNICALL Java_com_simplicity_maged_mccobjectdetection_services_NativeBlendInterface_blend(
		JNIEnv *jenv, jobject, jstring image1, jstring image2, jstring result,
		jint x, jint y) {
	try {
		// Read images : src image will be cloned into dst
		const char *img1 = jenv->GetStringUTFChars(image1, JNI_FALSE);

		Mat src = imread(img1, IMREAD_COLOR);
		LOGI("%s", img1);
		jenv->ReleaseStringUTFChars(image1, img1);

		const char *img2 = jenv->GetStringUTFChars(image2, JNI_FALSE);

		Mat dst = imread(img2, IMREAD_COLOR);
		LOGI("%s", img2);
		jenv->ReleaseStringUTFChars(image2, img2);

		Mat src_mask = Mat::zeros(src.rows, src.cols, src.depth());


/*		if (dst.isContinuous()) {
			int ss = dst.total() * dst.channels();
			LOGI("sizeInBytes if continuous data = %d", ss);
		}

		int  sizeInBytes = dst.step[0] * dst.rows;
		LOGI("sizeInBytes if not continuous data = %d", sizeInBytes);*/
		LOGI("=============1");
		// Define the mask as a closed polygon
		Point poly[1][7];
		poly[0][0] = Point(4, 80);
		poly[0][1] = Point(30, 54);
		poly[0][2] = Point(151, 63);
		poly[0][3] = Point(254, 37);
		poly[0][4] = Point(298, 90);
		poly[0][5] = Point(272, 134);
		poly[0][6] = Point(43, 122);

		const Point* polygons[1] = { poly[0] };
		int num_points[] = { 7 };
		LOGI("=============2");
		// Create mask by filling the polygon

		fillPoly(src_mask, polygons, num_points, 1, Scalar(255, 255, 255));
		LOGI("=============3");
		// The location of the center of the src in the dst
		Point center(x, y);

		// Seamlessly clone src into dst and put the results in output
		Mat output;
		seamlessClone(src, dst, src_mask, center, output, MIXED_CLONE);
		LOGI("=============4");
		const char *img3 = jenv->GetStringUTFChars(result, JNI_FALSE);
		LOGI("%s", img3);
		// Save result
		imwrite(img3, output);
		src.release();
		dst.release();
		output.release();
		LOGI("=============5");
		jenv->ReleaseStringUTFChars(result, img3);
	} catch (Exception& e) {
		LOGE("================ %s", e.what());
	}

}
}
