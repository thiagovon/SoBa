*/
float foo1 (float a, float b) {
  return sin(a) + b;
}/*

.text
.globl foo1

foo1:

# return sin(a) + b;
#temp = sin(a)

#temp += b

#return temp

