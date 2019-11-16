#include "battle_ui.h"

using namespace std;

BattleUI::BattleUI(tgui::Gui &gui)
{
    InitBattleTextGroup(gui);
    InitFightSwitchChoiceGroup(gui);
    InitPokemonSwitchGroup(gui);
    InitMoveChoiceGroup(gui);
    InitPokemonUI(gui);

    m_is_user_ready = false;
}

void BattleUI::InitBattleTextGroup(tgui::Gui &gui)
{
    m_btext_group = tgui::Group::create();
    gui.add(m_btext_group, "Battle Text Group");
    m_text_groups.push_back(m_btext_group);

    m_btext_label = tgui::Label::create();
    m_btext_label->setPosition("0%", "70%");
    m_btext_label->setSize("100%", "30%");
    m_btext_label->setText("TEXT HERE");   
    m_btext_label->setTextSize(text_size);
    m_btext_label->getRenderer()->setBackgroundColor(bg_color);
    m_btext_label->getRenderer()->setTextColor(text_color);
    m_btext_label->getRenderer()->setPadding(padding);

    // Set up signal
    void (BattleUI::*func)(bool);
    func = &BattleUI::SetIsUserReady;
    m_btext_label->connect("Clicked", func, this, true); 

    m_btext_group->add(m_btext_label);
}

void BattleUI::InitFightSwitchChoiceGroup(tgui::Gui &gui)
{
    // FIGHT-SWITCH CHOICE GROUP
    m_fs_group = tgui::Group::create();
    gui.add(m_fs_group, "Fight Switch Choice Group");
    m_text_groups.push_back(m_fs_group);
    m_fstext_label = tgui::Label::create();
    m_fstext_label->setPosition("0%", "70%");
    m_fstext_label->setSize("60%", "30%");
    m_fstext_label->setText("What will you do?");
    m_fstext_label->setTextSize(text_size);
    m_fstext_label->getRenderer()->setTextColor(text_color);
    m_fstext_label->getRenderer()->setBackgroundColor(bg_color);
    m_fstext_label->getRenderer()->setPadding(padding);
    m_fs_group->add(m_fstext_label);

    m_fsfight_label = tgui::Label::create();
    m_fsfight_label->setPosition("60%", "70%");
    m_fsfight_label->setSize("40%", "15%");
    m_fsfight_label->setText("FIGHT");
    m_fsfight_label->setTextSize(text_size);
    m_fsfight_label->getRenderer()->setTextColor(text_color);
    m_fsfight_label->getRenderer()->setBackgroundColor(bg_color);
    m_fsfight_label->getRenderer()->setPadding(padding);

    // Set up signal
    void (BattleUI::*f_func)(Command);
    f_func = &BattleUI::SetLastCommand;
    Command f_command("FIGHT", -1);
    m_fsfight_label->connect("Clicked", f_func, this, f_command);

    void (BattleUI::*a_func)(bool);
    a_func = &BattleUI::SetIsUserReady;
    m_fsfight_label->connect("Clicked", a_func, this, true);

    m_fs_group->add(m_fsfight_label);

    m_fsswitch_label = tgui::Label::create();
    m_fsswitch_label->setPosition("60%", "85%");
    m_fsswitch_label->setSize("40%", "15%");
    m_fsswitch_label->setText("SWITCH");
    m_fsswitch_label->setTextSize(text_size);
    m_fsswitch_label->getRenderer()->setTextColor(text_color);
    m_fsswitch_label->getRenderer()->setBackgroundColor(bg_color);
    m_fsswitch_label->getRenderer()->setPadding(padding);

    // Set up signal
    void (BattleUI::*s_func)(Command);
    s_func = &BattleUI::SetLastCommand;
    Command s_command("SWITCH", -1);
    m_fsswitch_label->connect("Clicked", s_func, this, s_command);

    void (BattleUI::*b_func)(bool);
    b_func = &BattleUI::SetIsUserReady;
    m_fsswitch_label->connect("Clicked", b_func, this, true);   

    m_fs_group->add(m_fsswitch_label);
}

void BattleUI::InitPokemonSwitchGroup(tgui::Gui &gui)
{
    m_switch_group = tgui::Group::create();
    gui.add(m_switch_group, "Pokemon Switch Group");
    m_text_groups.push_back(m_switch_group);

    m_switch_text_label = tgui::Label::create();
    m_switch_text_label->setPosition("0%", "70%");
    m_switch_text_label->setSize("30%", "30%");
    m_switch_text_label->setText("Switch to?");   
    m_switch_text_label->setTextSize(text_size);
    m_switch_text_label->getRenderer()->setBackgroundColor(bg_color);
    m_switch_text_label->getRenderer()->setTextColor(text_color);
    m_switch_text_label->getRenderer()->setPadding(padding);
    m_switch_group->add(m_switch_text_label);

    m_switch_back_label = tgui::Label::create();
    m_switch_back_label->setPosition("90%", "70%");
    m_switch_back_label->setSize("10%", "30%");
    m_switch_back_label->setText("Back");
    m_switch_back_label->setTextSize(text_size);
    m_switch_back_label->getRenderer()->setBackgroundColor(bg_color);
    m_switch_back_label->getRenderer()->setTextColor(text_color);
    m_switch_back_label->getRenderer()->setPadding(padding);

    // Set up signal
    void (BattleUI::*b_func)(Command);
    b_func = &BattleUI::SetLastCommand;
    Command b_command("BACK", -1);
    m_switch_back_label->connect("Clicked", b_func, this, b_command);

    void (BattleUI::*rb_func)(bool);
    rb_func = &BattleUI::SetIsUserReady;
    m_switch_back_label->connect("Clicked", rb_func, this, true);  

    m_switch_group->add(m_switch_back_label);

    for (int i = 0; i < 6; i++)
    {
        m_switch_pokemon_labels[i] = tgui::Label::create();
        m_switch_pokemon_labels[i]->setTextSize(text_size);
        m_switch_pokemon_labels[i]->setSize("30%", "10%");
        m_switch_pokemon_labels[i]->setText(to_string(i));
        m_switch_pokemon_labels[i]->getRenderer()->setBackgroundColor(bg_color);
        m_switch_pokemon_labels[i]->getRenderer()->setTextColor(text_color);
        m_switch_pokemon_labels[i]->getRenderer()->setPadding(padding);
        m_switch_group->add(m_switch_pokemon_labels[i], "Pokemon " + to_string(i + 1));

        // Set up signal
        void (BattleUI::*s_func)(Command);
        s_func = &BattleUI::SetLastCommand;
        Command s_command("SWITCH", i);
        m_switch_pokemon_labels[i]->connect("Clicked", s_func, this, s_command);

        void (BattleUI::*sb_func)(bool);
        sb_func = &BattleUI::SetIsUserReady;
        m_switch_pokemon_labels[i]->connect("Clicked", sb_func, this, true);  
    }

    m_switch_pokemon_labels[0]->setPosition("30%", "70%");
    m_switch_pokemon_labels[1]->setPosition("60%", "70%");
    m_switch_pokemon_labels[2]->setPosition("30%", "80%");
    m_switch_pokemon_labels[3]->setPosition("60%", "80%");
    m_switch_pokemon_labels[4]->setPosition("30%", "90%");
    m_switch_pokemon_labels[5]->setPosition("60%", "90%");
}

void BattleUI::InitPokemonUI(tgui::Gui &gui)
{
    m_player_pokemon_sprite.setPosition(50.f, 70.f);
    m_sprite_pointers.push_back(&m_player_pokemon_sprite);

    m_enemy_pokemon_sprite.setPosition(1000.f, 70.f);
    m_sprite_pointers.push_back(&m_enemy_pokemon_sprite);

    m_player_pokemon_hp_label = tgui::Label::create();
    m_player_pokemon_hp_label->setTextSize(text_size);
    m_player_pokemon_hp_label->setSize("30%", "10%");
    m_player_pokemon_hp_label->setPosition("10%", "0%");
    m_player_pokemon_hp_label->getRenderer()->setBackgroundColor(bg_color);
    m_player_pokemon_hp_label->getRenderer()->setTextColor(text_color);
    m_player_pokemon_hp_label->getRenderer()->setPadding(padding);

    m_enemy_pokemon_hp_label = tgui::Label::create();
    m_enemy_pokemon_hp_label->setTextSize(text_size);
    m_enemy_pokemon_hp_label->setSize("30%", "10%");
    m_enemy_pokemon_hp_label->setPosition("60%", "0%");
    m_enemy_pokemon_hp_label->getRenderer()->setBackgroundColor(bg_color);
    m_enemy_pokemon_hp_label->getRenderer()->setTextColor(text_color);
    m_enemy_pokemon_hp_label->getRenderer()->setPadding(padding);

    gui.add(m_player_pokemon_hp_label);
    gui.add(m_enemy_pokemon_hp_label);
}

void BattleUI::InitMoveChoiceGroup(tgui::Gui &gui)
{
    m_move_group = tgui::Group::create();
    gui.add(m_move_group, "Move Choice Group");
    m_text_groups.push_back(m_move_group);

    m_move_text_label = tgui::Label::create();
    m_move_text_label->setPosition("0%", "70%");
    m_move_text_label->setSize("30%", "30%");
    m_move_text_label->setText("Move select:");   
    m_move_text_label->setTextSize(text_size);
    m_move_text_label->getRenderer()->setBackgroundColor(bg_color);
    m_move_text_label->getRenderer()->setTextColor(text_color);
    m_move_text_label->getRenderer()->setPadding(padding);
    m_move_group->add(m_move_text_label);

    m_move_back_label = tgui::Label::create();
    m_move_back_label->setPosition("90%", "70%");
    m_move_back_label->setSize("10%", "30%");
    m_move_back_label->setText("Back");
    m_move_back_label->setTextSize(text_size);
    m_move_back_label->getRenderer()->setBackgroundColor(bg_color);
    m_move_back_label->getRenderer()->setTextColor(text_color);
    m_move_back_label->getRenderer()->setPadding(padding);

    // Set up signals for "back" button
    void (BattleUI::*b_func)(Command);
    b_func = &BattleUI::SetLastCommand;
    Command b_command("BACK", -1);
    m_move_back_label->connect("Clicked", b_func, this, b_command);

    void (BattleUI::*rb_func)(bool);
    rb_func = &BattleUI::SetIsUserReady;
    m_move_back_label->connect("Clicked", rb_func, this, true);  

    m_move_group->add(m_move_back_label);

    for (int i = 0; i < 4; i++)
    {
        m_move_labels[i] = tgui::Label::create();
        m_move_labels[i]->setTextSize(text_size);
        m_move_labels[i]->setSize("30%", "15%");
        m_move_labels[i]->setText(to_string(i));
        m_move_labels[i]->getRenderer()->setBackgroundColor(bg_color);
        m_move_labels[i]->getRenderer()->setTextColor(text_color);
        m_move_labels[i]->getRenderer()->setPadding(padding);
        m_move_group->add(m_move_labels[i], "Move " + to_string(i + 1));

        // Set up signal
        void (BattleUI::*m_func)(Command);
        m_func = &BattleUI::SetLastCommand;
        Command m_command("FIGHT", i);
        m_move_labels[i]->connect("Clicked", m_func, this, m_command);

        void (BattleUI::*rm_func)(bool);
        rm_func = &BattleUI::SetIsUserReady;
        m_move_labels[i]->connect("Clicked", rm_func, this, true);  
    }

    m_move_labels[0]->setPosition("30%", "70%");
    m_move_labels[1]->setPosition("60%", "70%");
    m_move_labels[2]->setPosition("30%", "85%");
    m_move_labels[3]->setPosition("60%", "85%");
}


// Hides all bottom text bar groups
void BattleUI::HideBottomBar()
{
    for (int i = 0; i < m_text_groups.size(); i++)
    {
        m_text_groups[i]->setVisible(false);
    }
}

// Displays the battle text widgets and waits for user input.
// message: What the label text is set to
void BattleUI::DisplayText(string message)
{
    HideBottomBar();
    m_btext_group->setVisible(true);
    m_btext_label->setText(message);

    WaitForUser();
}

// Displays the fight switch choice widgets and waits for user input.
void BattleUI::DisplayFSChoice()
{
    HideBottomBar();
    m_fs_group->setVisible(true);

    WaitForUser();
}

// Displays the Pokemon Choice widgets
// trainer: Pointer to the trainer who owns the pokemon party.
// pokemon_party: Array containing the pokemon the trainer owns.
// size: The size of the pokemon array.
void BattleUI::DisplayPChoice(Trainer *trainer, Pokemon pokemon_party[], int size)
{
    HideBottomBar();
    m_switch_group->setVisible(true);

    for (int i = 0; i < size; i++)
    {
        if (pokemon_party[i].IsValid())
        {
            sf::String name = pokemon_party[i].GetName();
            m_switch_pokemon_labels[i]->setVisible(true);
            if (trainer->IsLead(&pokemon_party[i]))
            {
                name = name + " (LEAD)";
            }
            if (pokemon_party[i].HasFainted())
            {
                name = name + " (FAINTED)";
            }
            tgui::Label::Ptr sss = m_switch_pokemon_labels[i];
            m_switch_pokemon_labels[i]->setText(name);
            m_switch_pokemon_labels[i]->getRenderer()->setBackgroundColor(bg_color);
        }
        else
        {
            m_switch_pokemon_labels[i]->setText("");
            m_switch_pokemon_labels[i]->getRenderer()->setBackgroundColor(inverse_color);
            m_switch_pokemon_labels[i]->setVisible(false);
        }
    }

    WaitForUser();
}

void BattleUI::DisplayMChoice()
{
    HideBottomBar();
    m_move_group->setVisible(true);

    for (int i = 0; i < 4; i++)
    {
        m_move_labels[i]->setText("Move " + to_string(i + 1));
    }

    WaitForUser();
}

// Waits until m_is_user_ready has been set to true before setting it to false and proceeding.
void BattleUI::WaitForUser()
{
    while (!GetIsUserReady())
    {
        continue;
    }
    SetIsUserReady(false);
    return;
}

void BattleUI::DrawAllSprites(sf::RenderWindow &window)
{
    for (int i = 0; i < m_sprite_pointers.size(); i++)
    {
        window.draw(*m_sprite_pointers[i]);
    }
}

void BattleUI::SetPlayerPokemonSprite(std::string image_name)
{
    if (!m_player_pokemon_texture.loadFromFile("images/" + image_name))
    {
        cout << "Cannot load image!" << endl;
        return;
    }
    m_player_pokemon_sprite.setTexture(m_player_pokemon_texture);
}

void BattleUI::SetEnemyPokemonSprite(std::string image_name)
{
    if (!m_enemy_pokemon_texture.loadFromFile("images/" + image_name))
    {
        cout << "Cannot load image!" << endl;
        return;
    }
    m_enemy_pokemon_sprite.setTexture(m_enemy_pokemon_texture);
}