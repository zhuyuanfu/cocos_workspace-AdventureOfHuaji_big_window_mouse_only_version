 #include "cocos2d.h"
class HuajiSprite :public cocos2d::Sprite
{
public:

	bool flying = true;
	bool on_the_ground = false;

	bool jump_from_ground = false;

	bool bullet_time = false;
	bool huaji_dead = false;

	//double gravity_acc_by_squareframe = -0.1;//这东西应该是场景的性质，因此将它从滑稽的成员变量中删除。
	double current_speed_x_pix_by_frame = 0;
	double current_speed_y_pix_by_frame = 0;
	double max_speed_x_pix_by_frame = 3;
	double max_speed_y_pix_by_frame = 6;

	double max_boost_speed = 40;
	double jump_from_ground_speed_y = 6;

	double max_bullet_time_detection_distance = 80;
	double max_bullet_hit_distance = 30;

	static HuajiSprite* create(const std::string & filename);

	HuajiSprite();
	~HuajiSprite();
};

