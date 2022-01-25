#!/usr/bin/python3
import os
import operator
from argparse import ArgumentParser

import xml.etree.ElementTree as ET

header = '''/* This file is generated by python scripts. Don't edit this file directly.
 */
'''

alphabet = "abcdefghijklmnopqrstuvwxyz"

eng_emojis = []
chs_emojis = []

def load_emoji(filename):
    tree = ET.parse(filename)
    root = tree.getroot()

    emojis = {}
    for annotation in root.findall('.//annotation'):
        for word in annotation.text.split('|'):
            word = word.strip()

            # only keep the first encountered emoji
            if not word in emojis:
                # print(annotation.get('cp'))
                emojis[word] = annotation.get('cp')

    return emojis

# no space allowed for English emoji

def filter_English_emoji(emojis):
    emojis_copy = {}

    for key, value in emojis.items():
        if ' ' in key:
            continue

        if len(key) > 6:
            continue

        # only accept alphabet
        isalphabet = True
        for c in key:
            if not c in alphabet:
                isalphabet = False

        if not isalphabet:
            continue

        #print(key, value)
        emojis_copy[key] = value

    return emojis_copy


# less than four characters for Chinese emoji

def filter_Chinese_emoji(emojis):
    emojis_copy = {}

    for key, value in emojis.items():
        if len(key) > 3:
            continue

        # just reject alphabet
        isalnum = False
        for c in key:
            if c.isdigit() or c in alphabet or c in alphabet.upper():
                isalnum = True

        if isalnum:
            continue

        #print(key, value)
        emojis_copy[key] = value

    return emojis_copy

def prepare_emojis():
    global eng_emojis, chs_emojis
    # need unicode cldr annotation from
    # /usr/share/unicode/cldr/common/annotations
    eng_emojis = filter_English_emoji(load_emoji('en.xml'))
    chs_emojis = filter_Chinese_emoji(load_emoji('yue_Hans.xml'))

    eng_emojis = [(key, value) for key, value in eng_emojis.items()]
    chs_emojis = [(key, value) for key, value in chs_emojis.items()]

    compare = operator.itemgetter(0)
    eng_emojis = sorted(eng_emojis, key=compare)
    chs_emojis = sorted(chs_emojis, key=compare)


def gen_english_emojis():
    entries = []
    for match, string in eng_emojis:
        match = '"{0}"'.format(match)
        entry = '{0:<10}, "{1}"'.format(match, string)
        entries.append(entry)
    return ',\n'.join(entries)

def gen_chinese_emojis():
    entries = []
    for match, string in chs_emojis:
        match = '"{0}"'.format(match)
        entry = '{0:<10}, "{1}"'.format(match, string)
        entries.append(entry)
    return ',\n'.join(entries)


def get_table_content(tablename):
    # English Emojis
    if tablename == 'ENGLISH_EMOJIS':
        return gen_english_emojis()
    # Chinese Emojis
    if tablename == 'CHINESE_EMOJIS':
        return gen_chinese_emojis()

def expand_file(filename):
    infile = open(filename, "r")
    print(header)
    for line in infile.readlines():
        line = line.rstrip(os.linesep)
        if len(line) < 3:
            print(line)
            continue
        if line[0] == '@' and line[-1] == '@':
            tablename = line[1:-1]
            print(get_table_content(tablename))
        else:
            print(line)


### main function ###
if __name__ == "__main__":
    parser = ArgumentParser(description='Generate header file from template.')
    parser.add_argument('infile', action='store', \
                        help='input file.')

    args = parser.parse_args()
    #print(args)

    prepare_emojis()
    expand_file(args.infile)
