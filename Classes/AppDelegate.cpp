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
	//�Ժ����ֻ�����ʾ�ֱ���Ϊ���Ŀ��1280x720
	glview->setFrameSize(720,1280);
#endif
	//glview->setFrameSize(640,960);
	//glview->setFrameSize(1080,1920);
	//��ԭ�еĳߴ�Ϊ640 960��ͼƬ��������ʾ�ߴ��С
	glview->setDesignResolutionSize(640,960, ResolutionPolicy::EXACT_FIT);
	//��windows������ʱ��0.5������ʾ ��Ԥ����������
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
