#include "AppDelegate.h"
#include "GameScene.h"

USING_NS_CC;

AppDelegate::AppDelegate() {

}

AppDelegate::~AppDelegate() 
{
}

bool AppDelegate::applicationDidFinishLaunching() {
    // initialize director
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    if(!glview) {
        glview = GLView::create("Ascending");
        director->setOpenGLView(glview);
    }

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	//以红米手机的显示分辨率为设计目标1280x720
	glview->setFrameSize(720,1280);
#endif
	//glview->setFrameSize(640,960);
	//glview->setFrameSize(1080,1920);
	//将原有的尺寸为640 960的图片拉伸至显示尺寸大小
	glview->setDesignResolutionSize(640,960, ResolutionPolicy::EXACT_FIT);
	//在windows下运行时按0.5比例显示 以预览整个界面
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	glview->setFrameZoomFactor(0.5);
#endif

    // turn on display FPS
	//director->setDisplayStats(true);
	director->setDisplayStats(false);

    // set FPS. the default value is 1.0/60 if you don't call this
    director->setAnimationInterval(1.0 / 60);

    // create a scene. it's an autorelease object
    auto scene = GameScene::createScene();

    // run
    director->runWithScene(scene);

    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
    Director::getInstance()->stopAnimation();

    // if you use SimpleAudioEngine, it must be pause
    // SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    Director::getInstance()->startAnimation();

    // if you use SimpleAudioEngine, it must resume here
    // SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
}
