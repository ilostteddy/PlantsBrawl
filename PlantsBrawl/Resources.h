#pragma once

#include <graphics.h>

#include "Atlas.h"

extern IMAGE img_menu_background;
extern IMAGE img_VS;                              // VS 艺术字图片
extern IMAGE img_1P;                              // 1P 文本图片
extern IMAGE img_2P;                              // 2P 文本图片
extern IMAGE img_1P_desc;                         // 1P 键位描述图片
extern IMAGE img_2P_desc;                         // 2P 键位描述图片
extern IMAGE img_gravestone_left;                 // 朝向左的墓碑图片
extern IMAGE img_gravestone_right;                // 朝向右的墓碑图片
extern IMAGE img_selector_tip;                    // 选角界面提示信息文本图片
extern IMAGE img_selector_background;             // 选角界面背景图
extern IMAGE img_1P_selector_btn_idle_left;       // 1P 向左选择按钮默认状态图片
extern IMAGE img_1P_selector_btn_idle_right;      // 1P 向右选择按钮默认状态图片
extern IMAGE img_1P_selector_btn_down_left;       // 1P 向左选择按钮按下状态图片
extern IMAGE img_1P_selector_btn_down_right;      // 1P 向右选择按钮按下状态图片
extern IMAGE img_2P_selector_btn_idle_left;       // 2P 向左选择按钮默认状态图片
extern IMAGE img_2P_selector_btn_idle_right;      // 2P 向右选择按钮默认状态图片
extern IMAGE img_2P_selector_btn_down_left;       // 2P 向左选择按钮按下状态图片
extern IMAGE img_2P_selector_btn_down_right;      // 2P 向右选择按钮按下状态图片
extern IMAGE img_peashooter_selector_background_left;  // 选角界面朝向左的豌豆射手背景图片
extern IMAGE img_peashooter_selector_background_right; // 选角界面朝向右的豌豆射手背景图片
extern IMAGE img_sunflower_selector_background_left;   // 选角界面朝向左的龙日葵背景图片
extern IMAGE img_sunflower_selector_background_right;  // 选角界面朝向右的龙日葵背景图片

extern IMAGE img_sky;                              // 天空图片
extern IMAGE img_hills;                            // 山脉图片
extern IMAGE img_platform_large;                   // 大型平台图片
extern IMAGE img_platform_small;                   // 小型平台图片

extern IMAGE img_1P_cursor;                        // 1P 指示光标图片
extern IMAGE img_2P_cursor;                        // 2P 指示光标图片

extern Atlas atlas_peashooter_idle_left;           // 豌豆射手朝向左的默认动画图集
extern Atlas atlas_peashooter_idle_right;          // 豌豆射手朝向右的默认动画图集
extern Atlas atlas_peashooter_run_left;            // 豌豆射手朝向左的奔跑动画图集
extern Atlas atlas_peashooter_run_right;           // 豌豆射手朝向右的奔跑动画图集
extern Atlas atlas_peashooter_attack_ex_left;      // 豌豆射手朝向左的特殊攻击动画图集
extern Atlas atlas_peashooter_attack_ex_right;     // 豌豆射手朝向右的特殊攻击动画图集
extern Atlas atlas_peashooter_die_left;            // 豌豆射手朝向左的死亡动画图集
extern Atlas atlas_peashooter_die_right;           // 豌豆射手朝向右的死亡动画图集

extern Atlas atlas_sunflower_idle_left;            // 龙日葵朝向左的默认动画图集
extern Atlas atlas_sunflower_idle_right;           // 龙日葵朝向右的默认动画图集
extern Atlas atlas_sunflower_run_left;             // 龙日葵朝向左的奔跑动画图集
extern Atlas atlas_sunflower_run_right;            // 龙日葵朝向右的奔跑动画图集
extern Atlas atlas_sunflower_attack_ex_left;       // 龙日葵朝向左的特殊攻击动画图集
extern Atlas atlas_sunflower_attack_ex_right;      // 龙日葵朝向右的特殊攻击动画图集
extern Atlas atlas_sunflower_die_left;             // 龙日葵朝向左的死亡动画图集
extern Atlas atlas_sunflower_die_right;            // 龙日葵朝向右的死亡动画图集

extern IMAGE img_pea;                              // 豌豆图片
extern Atlas atlas_pea_break;                      // 豌豆破碎动画图集
extern Atlas atlas_sun;                            // 日光动画图集
extern Atlas atlas_sun_explode;                    // 日光爆炸动画图集
extern Atlas atlas_sun_ex;                         // 特殊日光动画图集
extern Atlas atlas_sun_ex_explode;                 // 特殊日光爆炸动画图集
extern Atlas atlas_sun_text;                       // “日”文本动画图集

extern Atlas atlas_run_effect;                     // 奔跑特效动画图集
extern Atlas atlas_jump_effect;                    // 跳跃特效动画图集
extern Atlas atlas_land_effect;                     // 落地特效动画图集

extern IMAGE img_1P_winner;                       // 1P 获胜文本图片
extern IMAGE img_2P_winner;                       // 2P 获胜文本图片
extern IMAGE img_winner_bar;                      // 获胜玩家文本背景图片

extern IMAGE img_avatar_peashooter;                // 豌豆射手头像图片
extern IMAGE img_avatar_sunflower;                 // 龙日葵头像图片

void load_game_resources();
