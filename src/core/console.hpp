#pragma once

// Runs the commands right away as the server console would and returns what they printed.
plg::string ExecuteServerCommandCapture(std::string_view commands);
