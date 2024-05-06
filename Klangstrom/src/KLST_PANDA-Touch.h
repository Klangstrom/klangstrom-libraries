/*
 * Klangstrom
 *
 * This file is part of the *wellen* library (https://github.com/dennisppaul/wellen).
 * Copyright (c) 2024 Dennis P Paul.
 *
 * This library is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, version 3.
 *
 * This library is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef SRC_KLST_PANDA_TOUCH_H_
#define SRC_KLST_PANDA_TOUCH_H_

/* FT5206 capacitive touch panel controller */
/* interfaced via I2C4 ( which is shared with audio codec ) */

void touch_setup();
void touch_read();

#endif /* SRC_KLST_PANDA_TOUCH_H_ */
