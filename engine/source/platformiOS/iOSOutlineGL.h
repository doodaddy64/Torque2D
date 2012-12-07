//-----------------------------------------------------------------------------
// Torque
// Copyright GarageGames, LLC 2011
//-----------------------------------------------------------------------------

#ifndef __iOSOUTLINE__
#define __iOSOUTLINE__

#if defined(TORQUE_DEBUG)
#ifndef __GL_OUTLINE_FUNCS__
#define __GL_OUTLINE_FUNCS__

extern bool gOutlineEnabled;

extern void (* glDrawElementsProcPtr) (GLenum mode, GLsizei count, GLenum type, const GLvoid *indices);
extern void (* glDrawArraysProcPtr) (GLenum mode, GLint first, GLsizei count);

void glOutlineDrawElements(GLenum mode, GLsizei count, GLenum type, const GLvoid *indices);
void glOutlineDrawArrays(GLenum mode, GLint first, GLsizei count);

extern void (* glNormDrawElements) (GLenum mode, GLsizei count, GLenum type, const GLvoid *indices);
extern void (* glNormDrawArrays) (GLenum mode, GLint first, GLsizei count);

#ifndef NO_REDEFINE_GL_FUNCS
#define glDrawElements glDrawElementsProcPtr
#define glDrawArrays glDrawArraysProcPtr
#else 
#warning glDrawElements and glDrawArrays not redefined
#endif // NO_REDEFINE_GL_FUNCS
#endif // __GL_OUTLINE_FUNCS__
#endif // TORQUE_DEBUG

#endif // __iOSOUTLINE__

