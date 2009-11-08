import string

original = "g fmnc wms bgblr rpylqjyrc gr zw fylb. rfyrq ufyr amknsrcpq ypc dmp. bmgle gr gl zw fylb gq glcddgagclr ylb rfyr'q ufw rfgq rcvr gq qm jmle. sqgle qrpgle.kyicrpylq() gq pcamkkclbcb. lmu ynnjw ml rfc spj."

print string.translate(original,string.maketrans("abcdefghijklmnopqrstuvwxyz","cdefghijklmnopqrstuvwxyzab"))

print string.translate("map",string.maketrans("abcdefghijklmnopqrstuvwxyz","cdefghijklmnopqrstuvwxyzab"))+".html"
