#pragma once

#include "esp_console.h"

void cli_start(void);

class CliCommand
{
public:
    CliCommand(const char *command, const char *help, const char *hint,
               esp_console_cmd_func_t func, void *argtable)
    {
        m_cmd.command = command;
        m_cmd.help = help;
        m_cmd.hint = hint;
        m_cmd.func = func;
        m_cmd.argtable = argtable;
        esp_console_cmd_register(&m_cmd);
    }

private:
    esp_console_cmd_t m_cmd = {};
};
