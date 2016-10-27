# ---GLUE YOUR SHEEBANG HERE---
#
#                              Copyright (C) 2016 by Rafael Santiago
#
# This is free software. You can redistribute it and/or modify under
# the terms of the GNU General Public License version 2.
#
#

backtitle="/dev/enigma Setup"

l_rotor="i"
m_rotor="ii"
r_rotor="iii"
l_rotor_at="a"
m_rotor_at="a"
r_rotor_at="z"
reflector="c"
l_ring="1"
m_ring="1"
r_ring="1"
plugboard=""

l_rotor=$(dialog --stdout --backtitle "$backtitle" --radiolist "Select the left rotor:" 0 0 0 i "" on ii "" off iii "" off iv "" off v "" off vi "" off vii "" off viii "" off)

function abort() {
    echo "INFO: Aborted by the user."
    exit 1
}

if [ -z $l_rotor ]; then
    abort
fi

m_rotor=$(dialog --stdout --backtitle "$backtitle" --radiolist "Select the middle rotor:" 0 0 0 i "" off ii "" on iii "" off iv "" off v "" off vi "" off vii "" off viii "" off)

if [ -z $m_rotor ]; then
    abort
fi

r_rotor=$(dialog --stdout --backtitle "$backtitle" --radiolist "Select the right rotor:" 0 0 0 i "" off ii "" off iii "" on iv "" off v "" off vi "" off vii "" off viii "" off)

if [ -z $r_rotor ]; then
    abort
fi

l_rotor_at=$(dialog --stdout --backtitle "$backtitle" --radiolist "The left rotor will begin at:" 0 0 0 A "" on B "" off C "" off D "" off E "" off F "" off G "" off H "" off I "" off J "" off K "" off L "" off M "" off N "" off O "" off P "" off Q "" off R "" off S "" off T "" off U "" off V "" off W "" off X "" off Y "" off Z "" off)

if [ -z $l_rotor_at ]; then
    abort
fi

m_rotor_at=$(dialog --stdout --backtitle "$backtitle" --radiolist "The middle rotor will begin at:" 0 0 0 A "" on B "" off C "" off D "" off E "" off F "" off G "" off H "" off I "" off J "" off K "" off L "" off M "" off N "" off O "" off P "" off Q "" off R "" off S "" off T "" off U "" off V "" off W "" off X "" off Y "" off Z "" off)

if [ -z $m_rotor_at ]; then
    abort
fi

r_rotor_at=$(dialog --stdout --backtitle "$backtitle" --radiolist "The right rotor will begin at:" 0 0 0 A "" on B "" off C "" off D "" off E "" off F "" off G "" off H "" off I "" off J "" off K "" off L "" off M "" off N "" off O "" off P "" off Q "" off R "" off S "" off T "" off U "" off V "" off W "" off X "" off Y "" off Z "" off)

if [ -z $l_rotor_at ]; then
    abort
fi

reflector=$(dialog --stdout --backtitle "$backtitle" --radiolist "Select one reflector:" 0 0 0 B "" off C "" on)

if [ -z $reflector ]; then
    abort
fi

dialog --backtitle "$backtitle" --yesno "Do you want to set the left ring at position 1?" 0 0

if [ $? -eq 1 ]; then
    l_ring=$(dialog --stdout --backtitle "$backtitle" --radiolist "Select the new left ring position:" 0 0 0 "1" "" off "2" "" on "3" "" off "4" "" off "5" "" off "6" "" off "7" "" off "8" "" off "9" "" off "10" "" off "11" "" off "12" "" off "13" "" off "14" "" off "15" "" off "16" "" off "17" "" off "18" "" off "19" "" off "20" "" off "21" "" off "22" "" off "23" "" off "24" "" off "25" "" off "26" "" off)

    if [ -z $l_ring ]; then
        abort
    fi
fi

dialog --backtitle "$backtitle" --yesno "Do you want to set the middle ring at 1?" 0 0

if [ $? -eq 1 ]; then
    m_ring=$(dialog --stdout --backtitle "$backtitle" --radiolist "Select the new middle ring position:" 0 0 0 "1" "" off "2" "" on "3" "" off "4" "" off "5" "" off "6" "" off "7" "" off "8" "" off "9" "" off "10" "" off "11" "" off "12" "" off "13" "" off "14" "" off "15" "" off "16" "" off "17" "" off "18" "" off "19" "" off "20" "" off "21" "" off "22" "" off "23" "" off "24" "" off "25" "" off "26" "" off)

    if [ -z $m_ring ]; then
        abort
    fi
fi

dialog --backtitle "$backtitle" --yesno "Do you want to set the right ring at 1?" 0 0

if [ $? -eq 1 ]; then
    r_ring=$(dialog --stdout --backtitle "$backtitle" --radiolist "Select the new right ring position:" 0 0 0 "1" "" off "2" "" on "3" "" off "4" "" off "5" "" off "6" "" off "7" "" off "8" "" off "9" "" off "10" "" off "11" "" off "12" "" off "13" "" off "14" "" off "15" "" off "16" "" off "17" "" off "18" "" off "19" "" off "20" "" off "21" "" off "22" "" off "23" "" off "24" "" off "25" "" off "26" "" off)

    if [ -z $r_ring ]; then
        abort
    fi
fi

dialog --backtitle "$backtitle" --yesno "Do you want to let the plugboard without any swap?" 0 0

if [ $? -eq 1 ]; then
    plugboard=$(dialog --stdout --backtitle "$backtitle" --inputbox "So type the plugboard swaps here (S1/S1',...,S10/S10'):" 0 100)
fi

if [ -z $plugboard ]; then
    enigmactl --set --l-rotor=$l_rotor --m-rotor=$m_rotor --r-rotor=$r_rotor --l-rotor-at=$l_rotor_at --m-rotor-at=$m_rotor_at --r-rotor-at=$r_rotor_at --reflector=$reflector --l-ring=$l_ring --m-ring=$m_ring --r-ring=$r_ring
else
    enigmactl --set --l-rotor=$l_rotor --m-rotor=$m_rotor --r-rotor=$r_rotor --l-rotor-at=$l_rotor_at --m-rotor-at=$m_rotor_at --r-rotor-at=$r_rotor_at --reflector=$reflector --l-ring=$l_ring --m-ring=$m_ring --r-ring=$r_ring --plugboard=$plugboard
fi

