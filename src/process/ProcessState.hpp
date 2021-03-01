/**
 * Process State - Header File
 * 
 * @author Tibor Buzási <develop@tiborbuzasi.com>
 * 
 * Copyright © 2020 Tibor Buzási. All rights reserved.
 * For licensing information see LICENSE in the project root folder.
 */

#ifndef SOWACO_PROCESS_STATE_H
#define SOWACO_PROCESS_STATE_H

enum ProcessState
{
    WAITING,
    PIPE_DRAINING,
    COLLECTING,
    DRAINING,
    PARTIAL_DRAINING
};

#endif // SOWACO_PROCESS_STATE_H