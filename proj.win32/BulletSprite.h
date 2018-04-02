#include "CCSprite.h"

class BulletSprite :public cocos2d::Sprite{
public:
	double speed = 4;
	double current_speed_x = -4;
	double current_speed_y = 0;
	
	//int position_launch_x = 0;
	//int position_launch_y = 0;

	double angle_head_in_rectangular_coordinates = 90;
	double angle_head_in_game_world = 0;

	int launch_position_x = 800;
	int launch_position_y = 800;

	bool harmful_to_huaji = true;

	double max_boost_speed = 24;
	
	bool friendly = false;//Èç¹û½ø¹¥

	bool alive = true;
	bool exploded = false;

	static BulletSprite* create(const std::string & filename); 
	BulletSprite();
	~BulletSprite();
};

