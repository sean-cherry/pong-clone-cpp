#define is_down(b) input->buttons[b].is_down
#define pressed(b) input->buttons[b].is_down && input->buttons[b].changed
#define released(b) (!input->buttons[b].is_down && input->buttons[b].changed)

float player1_p, player1_dp;
float player2_p, player2_dp;
float arena_half_size_x = 85;
float arena_half_size_y = 45;
float player_half_size_x = 2;
float player_half_size_y = 12;
float ball_p_x, ball_p_y, ball_dp_x = 80, ball_dp_y, ball_half_size = 2;
int player1_score = 0;
int player2_score = 0;
int hitcounter = 0;

internal void simulate_game(Input* input, float dt) {
	clear_screen(0x39ff14);	//border
	draw_rect(0, 0, arena_half_size_x, arena_half_size_y, 0x000000); //arena
	//player movement
	{
		//player 1
		float player1_ddp = 0.f;
		float player2_ddp = 0.f;

		if (is_down(BUTTON_UP)) player1_ddp += 1750;
		if (is_down(BUTTON_DOWN)) player1_ddp -= 1750;

		if (player1_p + player_half_size_y > arena_half_size_y) {
			player1_p = arena_half_size_y - player_half_size_y;
			player1_dp *= 0;
		} else if (player1_p - player_half_size_y < -arena_half_size_y) {
			player1_p = -arena_half_size_y + player_half_size_y;
			player1_dp *= 0;
		}

		player1_ddp -= player1_dp * 10.f;

		player1_p = player1_p + player1_dp * dt + player1_ddp * dt * dt * .5f;
		player1_dp = player1_dp + player1_ddp * dt;

		//player 2
		if (is_down(BUTTON_W)) player2_ddp += 2000;
		if (is_down(BUTTON_S)) player2_ddp -= 2000;

		if (player2_p + player_half_size_y > arena_half_size_y) {
			player2_p = arena_half_size_y - player_half_size_y;
			player2_dp *= 0;
		}
		else if (player2_p - player_half_size_y < -arena_half_size_y) {
			player2_p = -arena_half_size_y + player_half_size_y;
			player2_dp *= 0;
		}

		player2_ddp -= player2_dp * 10.f;

		player2_p = player2_p + player2_dp * dt + player2_ddp * dt * dt * .5f;
		player2_dp = player2_dp + player2_ddp * dt;
	}
	//ball movement
	{
		ball_p_x += ball_dp_x * dt;
		ball_p_y += ball_dp_y * dt;

		draw_rect(ball_p_x, ball_p_y, 2, 2, 0xffffff);

		if (ball_p_x + ball_half_size > 80 - player_half_size_x &&
			ball_p_x - ball_half_size < 80 + player_half_size_x &&
			ball_p_y + ball_half_size > player1_p - player_half_size_y &&
			ball_p_y - ball_half_size < player1_p + player_half_size_y) {
			ball_p_x = 80 - player_half_size_x - ball_half_size;
			ball_dp_x *= -1;
			ball_dp_y = 0.75 * player1_dp + (player1_p - ball_p_y)*2;
			//Beep(880, 100);
		}
		else if (ball_p_x + ball_half_size > -80 - player_half_size_x &&
			ball_p_x - ball_half_size < -80 + player_half_size_x &&
			ball_p_y + ball_half_size > player2_p - player_half_size_y &&
			ball_p_y - ball_half_size < player2_p + player_half_size_y) {
			ball_p_x = -80 + player_half_size_x + ball_half_size;
			ball_dp_x *= -1;
			ball_dp_y = 0.75 * player2_dp + (player2_p - ball_p_y)*2;
			//Beep(880, 100);
		}

		if (ball_p_y + ball_half_size > arena_half_size_y) {
			ball_p_y = arena_half_size_y - ball_half_size;
			ball_dp_y *= -1;
			//Beep(294, 100);
		}
		else if (ball_p_y - ball_half_size < -arena_half_size_y) {
			ball_p_y = -arena_half_size_y + ball_half_size;
			ball_dp_y *= -1;
			//Beep(294, 100);
		}
	}
	//goal
	{
		if (ball_p_x + ball_half_size > arena_half_size_x) {
			ball_dp_x *= -1;
			ball_dp_y = 0;
			ball_p_x = 0;
			ball_p_y = 0;
			player1_score += 1;
			//Beep(164, 500);
		}
		else if (ball_p_x - ball_half_size < -arena_half_size_x) {
			ball_dp_x *= -1;
			ball_dp_y = 0;
			ball_p_x = 0;
			ball_p_y = 0;
			player2_score += 1;
			//Beep(164, 500);
		}
	}
	draw_rect(80, player1_p, player_half_size_x, player_half_size_y, 0x39ff14);

	draw_rect(-80, player2_p, player_half_size_x, player_half_size_y, 0x39ff14);
}