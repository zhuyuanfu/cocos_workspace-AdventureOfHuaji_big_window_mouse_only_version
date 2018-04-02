#include "cocos2d.h"
#include "BulletSprite.h"
class BossSprite :public cocos2d::Sprite
{
public:

	bool flying = true;
	bool on_the_ground = false;

	bool jump_from_ground = false;

	//bool bullet_time = false;//�ⶫ��Ӧ���ǳ��������ʣ���˽����ӻ����ĳ�Ա������ɾ����
	//bool huaji_dead = false;

	//double gravity_acc_by_squareframe = -0.1;//�ⶫ��Ӧ���ǳ��������ʣ���˽����ӻ����ĳ�Ա������ɾ����
	double current_speed_x_pix_by_frame = 0;
	double current_speed_y_pix_by_frame = 0;
	double max_speed_x_pix_by_frame = 1;
	double max_speed_y_pix_by_frame = 5;

	int hp = 3;
	//double max_boost_speed = 20;

	static BossSprite* create(const std::string & filename);

	void fireABullet(BulletSprite* bulletSprite);

	BossSprite();
	~BossSprite();
};

