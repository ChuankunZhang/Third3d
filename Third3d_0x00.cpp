
#include<stdio.h>
#include<math.h>
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                //
//  ____________ _        _                                                                                       //
// |_____ ______| |      | |                 ==========        ||=======\\          __ ===                        //
//      | |     | |      | |                           \\      ||        \\        /  \ =====                     //
//      | |     | |      | |                            \\     ||        ||       /    \ ========                 //
//      | |     | |______| |      ___   __              //     ||        ||      / __   \ ============            //
//      | |     |  ______  | === |   | |  \  ===========       ||        ||     | |  |  | ================        //
//      | |     | |      | |  |  |___| |  |             \\     ||        ||     | |__|  | ============            //
//      | |     | |      | |  |  |\    |  |             //     ||        ||      \      /========                 //
//      | |     | |      | |  |  | \   |  |            //      ||        //       \    /=====                     //
//      |_|     |_|      |_| === |  \  |__/  ==========        ||=======//         \__/===                        //
//                                                                                                  version:0x00  //
//                                                                                                                //
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef _t3dDef
	#define _t3dDef
	#define null 0
	#define pai 3.141592f
#endif
#ifndef _t3d
	#define _t3d
#ifndef _t3dCollidePoolSize
	#define _t3dCollidePoolSize 	65536
#endif
#ifndef _t3dOctreePoolSize
	#define _t3dOctreePoolSize 	65536
#endif
typedef float t3dfloat;
typedef void*(*t3dcallback1)(void*);
typedef void*(*t3dcallback2)(void*,void*);
typedef void*(*t3dcallback3)(void*,void*,void*);
typedef void*(*t3dcallback4)(void*,void*,void*,void*);
namespace Third3d{
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
#ifndef _t3dTmp
#define _t3dTmp
namespace tmp{
	template<typename T>class array{
		public:
			void 	write(int n,T d){
				if(n<=l)
					data[n]=d;
				else
					return;
			}
			T 	read(int n){
				if(n<=l)
					return data[n];
				else
					return null;
			}
			T * 	readptr(int n){
				if(n<=l)
					return &data[n];
				else
					return null;
			}
			T 	operator[](int le){
				return this->read(le);
			}
			void 	foreach(void(*callback)(T*,void*),void *a){
				int i;
				for(i=0;i<=l;i++){
					callback(&data[i],a);
				}
			}
			void 	push(T d){
				T *	l2;
				int 	j;
				if(lm>l){
					l++;
					data[l]=d;
				}else{
					l2=new T[lm*2];
					for(j=0;j<=lm;j++){
						l2[j]=data[j];
					}
					lm=lm*2;
					delete[] data;
					data=l2;
					l++;
					data[l]=d;
				}
			}
			T 	pop(){
				if(l==0) return null;
				l--;
				return data[l+1];
			}
			void 	clean(){
				int 	i;
				T *	d2;
				d2=new T[l];
				for(i=0;i<=l;i++){
					d2[i]=data[i];
				}
				delete data;
				lm=l;
				data=d2;
			}
			void init(int n){
				data=new T[n];
				l=0;
				lm=n;
			}
			int length(){
				return l;
			}
			array(){
				this->init(2);
			}
			array(int n){
				this->init(n);
			}
			~array(){
				delete[] data;
			}
		private:
			int 	l;
			int 	lm;
			T *	data;
	};
	template<typename T>class table{
		public:
			struct info{
				bool 	del;
				T *	data;
				int 	id;
				info(){
					id=0;
					del=false;
				}
			};
			class line{
				public:
				bool 	isdel;
				T 	data;
				line(){
					isdel=true;
				}
				void operator=(line *l){
					data=l->data;
					isdel=l->isdel;
				}
				void operator=(T d){
					data=d;
					isdel=false;
				}
			};
			void 	foreach(void(*callback)(info * s,void *a),void *a){
				info s;
				for(s.id=0;s.id<=ln;s.id++){
					if(!l[s.id].isdel){
						s.data=&l[s.id].data;
						s.del=false;
						callback(&s,a);
						if(s.del) l[s.id].isdel=true;
					}
				}
			}
			T *	findById(int id){
				if(l[id].isdel){
					throw 0x00;
					return null;
				}
				return &l[id].data;
			}
			void 	update(int id,T data){
				if(l[id].isdel){
					throw 0x00;
					return;
				}
				l[id].data=data;
			}
			int 	insert(T data){
				int 	i,j;
				line *	l2;
				for(i=0;i<=ln;i++){
					if(l[i].isdel){
						l[i].isdel=false;
						l[i].data=data;
						return i;
					}
				}
				i++;
				l2=new line[ln*2];
				for(j=0;j<=ln;j++){
					l2[j].data=l[j].data;
					l2[j].isdel=l[j].isdel;
				}
				ln=ln*2;
				delete[] l;
				l=l2;
				l2[i].data=data;
				l2[i].isdel=false;
			}
			void 	del(int id){
				l[id].isdel=true;
			}
			void 	deldata(T d){
				int i;
				for(i=0;i<=ln;i++){
					if(l[i].isdel) continue;
					if(l[i].data==d) l[i].isdel=true;
				}
			}
			table(){
				l=new line[2];
				ln=2;
			}
			~table(){
				delete[] l;
			}
		private:
			line *	l;
			int 	ln;
	};
}
#endif
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
#ifndef _t3dOctree
#define _t3dOctree
#define _PAR 0x00
#define _TFL 0x01
#define _TFR 0x02
#define _TBL 0x03
#define _TBR 0x04
#define _BFL 0x05
#define _BFR 0x06
#define _BBL 0x07
#define _BBR 0x08
template<typename T> class vec3{
	public:
		T x;
		T y;
		T z;
		vec3(){
			x=0.0f;
			y=0.0f;
			z=0.0f;
		}
		vec3(T xt,T yt,T zt){
			x=xt;
			y=yt;
			z=zt;
		}
		void init(T xt,T yt,T zt){
			x=xt;
			y=yt;
			z=zt;
		}
		void operator=(vec3<T> *p){
			x=p->x;
			y=p->y;
			z=p->z;
		}
		T norm(){
			return sqrt((x*x)+(y*y)+(z*z));
		}
		T pro(vec3<T> *p){
			return (
				(x*p->x)+
				(y*p->y)+
				(z*p->z)
			);
		}
};
class octree{
	public:
		struct thing;
		struct pool;
		struct node{
			void *	child;
			bool	isTree;
			void 	init		();
			void 	delthing	();
		};
		struct tree{
			pool *	Pool;
			node 	tfl;
			node 	tfr;
			node 	tbl;
			node 	tbr;
			node 	bfl;
			node 	bfr;
			node 	bbl;
			node 	bbr;
			tree *	parent;
			thing *	other;
			char 	parposi;
			void 	destruct	();
			void 	init		();
			tree ** posi		(char p);
			bool 	isEmpty		();
			bool 	trydest		();
		};
		struct pool{
			octree *par;
			pool *	next;
			tree 	data[_t3dOctreePoolSize];
			int  	used[_t3dOctreePoolSize];
			//int 	length;
			int 	time;
			int 	num;
			pool(){
				//data	=null;
				//length=0;
				time	=0;
				//used	=null;
				next	=null;
				num	=0;
				usednum=0;
			}
			~pool(){
				//if(data) 	delete[] 	data;
				//if(used) 	delete[] 	used;
				if(next) 	delete 		next;
			}
			void 	poolinit();
			//void 	poolinit(int l);
			void 	nextinit();
			tree *	construct();
			void	destruct	(tree * d);
			void	clear();
			private:
			int	li;
			int	usednum;
		};
		struct thing{
			vec3<t3dfloat> 	position;
			void *		child;
			thing *		last;
			thing *		next;
			thing(){
				last=null;
				next=null;
			}
			struct octbuf{
				tree *	Tree;
				vec3<t3dfloat>	position;
				t3dfloat	length;
				int 		deep;
				octbuf():position(){
					Tree=null;
					length=0.0f;
					deep=0;
				}
			}octreebuffer;
			void update();
			void remove();
		};
		t3dfloat 	length;
		tree *		root;
		octree(){
			Pool=new pool();
			Pool->poolinit();
			Pool->par=this;
			root=Pool->construct();
			Pool->num++;
			maxdeep=64;
		}
		octree(octree *o){
			Pool	=o->Pool;
			root	=o->root;
			length	=o->length;
			pp.x	=o->pp.x;
			pp.y	=o->pp.y;
			pp.z	=o->pp.z;
			pt	=o->pt;
			pl	=o->pl;
			Pool->num++;
			maxdeep	=o->maxdeep;
		}
		~octree(){
			if(Pool==null)
				return;
			Pool->num--;
			if(Pool->num<=0)
				delete Pool;
		}
		tree *		construct	();
		void 		destruct	(tree *d);
		void 		push		(thing * d);
		void 		push		(thing *o,tree *t,t3dfloat x,t3dfloat y,t3dfloat z,t3dfloat l);
		tree *		pt;		//point
		vec3<t3dfloat> 	pp;		//point position
		t3dfloat	pl;		//point length
		int		pd;		//deep
		int		maxdeep;
		void 		p_init		();
		void 		p_go		(char p);
		void 		p_goparent	();
		bool 		p_isin		(thing *o);
		template<typename T>
		void 		foreach		(T *c);
		template<typename T>
		void 		search		(t3dfloat x,t3dfloat y,t3dfloat z,t3dfloat l,T *c);
		void 		clear		();
		void 		operator=	(octree *o);
	private:
		pool *		Pool;
		template<typename T>
		void 		foreachchild	(tree *t,t3dfloat x,t3dfloat y,t3dfloat z,t3dfloat l,T *c);
};
//////////////////////////////////////////////
/////////////////////functions////////////////
//////////////////////////////////////////////
void 	octree::operator=(octree *o){
	this->~octree();
	*this=octree(o);
}
void 	octree::pool::poolinit(){
	int i;
	if(data!=null) return;
	//length=l;
	//data=new octree::tree	[l];
	//used=new int		[l];
	for(i=0;i<=_t3dOctreePoolSize;i++){
		used[i]=-1;
	}
}
void 	octree::pool::nextinit(){
	if(next) return;
	next=new octree::pool;
	next->poolinit();
	next->par=par;
}
octree::tree *	octree::pool::construct(){
	octree::tree * 	d;
	int 		i;
	if(data==null) poolinit();
	if(usednum>_t3dOctreePoolSize) goto goNextPage;
	for(i=li;i<=_t3dOctreePoolSize;i++){
		if(used[i]==time) continue;
		used[i]=time;
		data[i].init();
		li=i;
		usednum++;
		return &data[i];
	}
	if(li!=0)
	for(i=0;i<=li;i++){
		if(used[i]==time) continue;
		used[i]=time;
		data[i].init();
		li=i;
		usednum++;
		return &data[i];
	}
	goNextPage:
	li=0;
	nextinit();
	return next->construct();
}
void	octree::pool::destruct(octree::tree *d){
	int i;
	i=d-data;
	if(i>_t3dOctreePoolSize || i<0){
		if(next)
			next->destruct(d);
		return;
	}
	used[i]--;
	usednum--;
}
void 	octree::pool::clear(){
	time++;
	usednum=0;
	li=0;
	if(next) next->clear();
}
void 	octree::clear(){
	root=null;
	Pool->clear();
	root=Pool->construct();
}
octree::tree *	octree::construct(){
	return Pool->construct();
}
void 		octree::destruct(octree::tree *d){
	Pool->destruct(d);
}
void 	octree::p_init(){
	pp.init(0.0f,0.0f,0.0f);
	pt=root;
	pl=length;
	pd=0;
}
void 	octree::p_goparent(){
	t3dfloat 	le;
	le=pl*2;
	if(pt->parent==null) return;
	switch(pt->parposi){
		case _TFL :
			pp.x-=le;
			pp.z-=le;
		break;
		 case _TFR :
			pp.x-=le;
			pp.y-=le;
			pp.z-=le;
		break;
		 case _TBL :
			pp.z-=le;
		break;
		 case _TBR :
			pp.y-=le;
			pp.z-=le;
		break;
		 case _BFL :
			pp.x-=le;
		break;
		 case _BFR :
			pp.x-=le;
			pp.y-=le;
		break;
		 case _BBL :
		break;
		 case _BBR :
			pp.y-=le;
		break;
		default:return;
	}
	pl=le;
	pd--;
	pt=pt->parent;
}
void 	octree::p_go(char p){
	octree::tree **ptt;
	ptt=pt->posi(p);
	if(*ptt==null) return;
	switch(p){
		 case _PAR:
			p_goparent();
		return;
		 case _TFL :
			pp.x+=pl;
			pp.z+=pl;
		break;
		 case _TFR :
			pp.x+=pl;
			pp.y+=pl;
			pp.z+=pl;
		break;
		 case _TBL :
			pp.z+=pl;
		break;
		 case _TBR :
			pp.y+=pl;
			pp.z+=pl;
		break;
		 case _BFL :
			pp.x+=pl;
		break;
		 case _BFR :
			pp.x+=pl;
			pp.y+=pl;
		break;
		 case _BBL :
		break;
		 case _BBR :
			pp.y+=pl;
		break;
		default:return;
	}
	pl=pl/2;
	pd++;
	pt=*ptt;
}
bool 	octree::p_isin(octree::thing *o){
	if(
		o->position.x>pp.x || 
		o->position.y>pp.y || 
		o->position.z>pp.z || 
		o->position.x<pp.x+pl || 
		o->position.y<pp.y+pl || 
		o->position.z<pp.z+pl
	)
		return true;
	else
		return false;
}
void 	octree::node::init(){
	child=null;
}
void 	octree::node::delthing(){
	if(!isTree)
	if(child)
	((thing *)child)->octreebuffer.Tree=null;
	child=null;
}
void 	octree::tree::destruct(){
	tfl.delthing();
	tfr.delthing();
	tbl.delthing();
	tbr.delthing();
	bfl.delthing();
	bfr.delthing();
	bbl.delthing();
	bbr.delthing();
	if(tfl.child!=null) ((octree::tree *)tfl.child)->destruct();
	if(tfr.child!=null) ((octree::tree *)tfr.child)->destruct();
	if(tbl.child!=null) ((octree::tree *)tbl.child)->destruct();
	if(tbr.child!=null) ((octree::tree *)tbr.child)->destruct();
	if(bfl.child!=null) ((octree::tree *)bfl.child)->destruct();
	if(bfr.child!=null) ((octree::tree *)bfr.child)->destruct();
	if(bbl.child!=null) ((octree::tree *)bbl.child)->destruct();
	if(bbr.child!=null) ((octree::tree *)bbr.child)->destruct();
	Pool->destruct(this);
}
void 	octree::tree::init(){
	tfl.init();
	tfr.init();
	tbl.init();
	tbr.init();
	bfl.init();
	bfr.init();
	bbl.init();
	bbr.init();
	parent	=null;
	parposi	=null;
	Pool	=null;
	other	=null;
}
octree::tree ** octree::tree::posi(char p){
	octree::node * n;
	switch(p){
		 case _TFL :n=&tfl;	break;
		 case _TFR :n=&tfr;	break;
		 case _TBL :n=&tbl;	break;
		 case _TBR :n=&tbr;	break;
		 case _BFL :n=&bfl;	break;
		 case _BFR :n=&bfr;	break;
		 case _BBL :n=&bbl;	break;
		 case _BBR :n=&bbr;	break;
		default:return null;
	}
	if(n->isTree) return (  (octree::tree **) &n->child  );
	return null;
}
bool 	octree::tree::isEmpty(){
	if(tfl.child==null)
	if(tfr.child==null)
	if(tbl.child==null)
	if(tbr.child==null)
	if(bfl.child==null)
	if(bfr.child==null)
	if(bbl.child==null)
	if(bbr.child==null)
	return true;
	return false;
}
bool 	octree::tree::trydest(){
	if(!isEmpty()) return false;
	if(parent){
		*(parent->posi(parposi))=null;
		parent->trydest();
	}
	destruct();
	return true;
}
void 	octree::push(octree::thing *d){
	d->octreebuffer.deep=pd;
	push(d,root,pp.x,pp.y,pp.z,pl);
}
void 	octree::push(octree::thing *o,octree::tree *t,t3dfloat x,t3dfloat y,t3dfloat z,t3dfloat l){
	octree::thing 	*n2;
	vec3<t3dfloat> 	p;
	bool		maxmode;
	if(!p_isin(o)){
		return;
		throw 0x01;
	}
	if(o->octreebuffer.Tree) {
		throw 0x05;
		return;
	}
	if(o->octreebuffer.deep>maxdeep){
		maxmode=true;
		o->last=null;
		o->next=t->other;
		if(t->other)t->other->last=o;
		t->other=o;
		return;
	}else
		maxmode=false;
	if((o->position.x)>(x+l)){
		if((o->position.y)>(y+l)){
			if((o->position.z)>(z+l)){
				//tfr
						p.init(x,y,z);
						p.x=x+l;
						p.y=y+l;
						p.z=z+l;
						o->octreebuffer.position.x=p.x;
						o->octreebuffer.position.y=p.y;
						o->octreebuffer.position.z=p.z;
						o->octreebuffer.length=l/2;
						o->octreebuffer.Tree=t;
						o->octreebuffer.deep++;
				if(t->tfr.child==null){
					t->tfr.isTree=false;
					t->tfr.child=o;
					return;
				}else{
					if(t->tfr.isTree){
						push(o,(octree::tree *)t->tfr.child,p.x,p.y,p.z,l/2);
						return;
					}else{
						n2=(octree::thing *)t->tfl.child;
						t->tfr.child=construct();
						t->tfr.isTree=true;
						push(n2,(octree::tree *)t->tfr.child,p.x,p.y,p.z,l/2);
						push(o,(octree::tree *)t->tfr.child,p.x,p.y,p.z,l/2);
						return;
					}
				}
			}else{
				//bfr
						p.init(x,y,z);
						p.x=x+l;
						p.y=y+l;
						o->octreebuffer.position.x=p.x;
						o->octreebuffer.position.y=p.y;
						o->octreebuffer.position.z=p.z;
						o->octreebuffer.length=l/2;
						o->octreebuffer.Tree=t;
						o->octreebuffer.deep++;
				if(t->bfr.child==null){
					t->bfr.isTree=false;
					t->bfr.child=o;
					return;
				}else{
					if(t->bfr.isTree){
						push(o,(octree::tree *)t->bfr.child,p.x,p.y,p.z,l/2);
						return;
					}else{
						n2=(octree::thing *)t->bfl.child;
						t->bfr.child=construct();
						t->bfr.isTree=true;
						push(n2,(octree::tree *)t->bfr.child,p.x,p.y,p.z,l/2);
						push(o,(octree::tree *)t->bfr.child,p.x,p.y,p.z,l/2);
						return;
					}
				}
			}
		}else{
			if((o->position.z)>(z+l)){
				//tfl
						p.init(x,y,z);
						p.x=x+l;
						p.z=z+l;
						o->octreebuffer.position.x=p.x;
						o->octreebuffer.position.y=p.y;
						o->octreebuffer.position.z=p.z;
						o->octreebuffer.length=l/2;
						o->octreebuffer.Tree=t;
						o->octreebuffer.deep++;
				if(t->tfl.child==null){
					t->tfl.isTree=false;
					t->tfl.child=o;
					return;
				}else{
					if(t->tfl.isTree){
						push(o,(octree::tree *)t->tfl.child,p.x,p.y,p.z,l/2);
						return;
					}else{
						n2=(octree::thing *)t->tfl.child;
						t->tfl.child=construct();
						t->tfl.isTree=true;
						push(n2,(octree::tree *)t->tfl.child,p.x,p.y,p.z,l/2);
						push(o,(octree::tree *)t->tfl.child,p.x,p.y,p.z,l/2);
						return;
					}
				}
			}else{
				//bfl
						p.init(x,y,z);
						p.x=x+l;
						o->octreebuffer.position.x=p.x;
						o->octreebuffer.position.y=p.y;
						o->octreebuffer.position.z=p.z;
						o->octreebuffer.length=l/2;
						o->octreebuffer.Tree=t;
						o->octreebuffer.deep++;
				if(t->bfl.child==null){
					t->bfl.isTree=false;
					t->bfl.child=o;
					return;
				}else{
					if(t->bfl.isTree){
						push(o,(octree::tree *)t->bfl.child,p.x,p.y,p.z,l/2);
						return;
					}else{
						n2=(octree::thing *)t->bfl.child;
						t->bfl.child=construct();
						t->bfl.isTree=true;
						push(n2,(octree::tree *)t->bfl.child,p.x,p.y,p.z,l/2);
						push(o,(octree::tree *)t->bfl.child,p.x,p.y,p.z,l/2);
						return;
					}
				}
			}
		}
	}else{
		if((o->position.y)>(y+l)){
			if((o->position.z)>(z+l)){
				//tbr
						p.init(x,y,z);
						p.y=y+l;
						p.z=x+l;
						o->octreebuffer.position.x=p.x;
						o->octreebuffer.position.y=p.y;
						o->octreebuffer.position.z=p.z;
						o->octreebuffer.length=l/2;
						o->octreebuffer.Tree=t;
						o->octreebuffer.deep++;
				if(t->tbr.child==null){
					t->tbr.isTree=false;
					t->tbr.child=o;
					return;
				}else{
					if(t->tbr.isTree){
						push(o,(octree::tree *)t->tbr.child,p.x,p.y,p.z,l/2);
						return;
					}else{
						n2=(octree::thing *)t->tbr.child;
						t->tbr.child=construct();
						t->tbr.isTree=true;
						push(n2,(octree::tree *)t->tbr.child,p.x,p.y,p.z,l/2);
						push(o,(octree::tree *)t->tbr.child,p.x,p.y,p.z,l/2);
						return;
					}
				}
			}else{
				//bbr
						p.init(x,y,z);
						p.y=y+l;
						o->octreebuffer.position.x=p.x;
						o->octreebuffer.position.y=p.y;
						o->octreebuffer.position.z=p.z;
						o->octreebuffer.length=l/2;
						o->octreebuffer.Tree=t;
						o->octreebuffer.deep++;
				if(t->bbr.child==null){
					t->bbr.isTree=false;
					t->bbr.child=o;
					return;
				}else{
					if(t->bbr.isTree){
						push(o,(octree::tree *)t->bbr.child,p.x,p.y,p.z,l/2);
						return;
					}else{
						n2=(octree::thing *)t->bbr.child;
						t->bbr.child=construct();
						t->bbr.isTree=true;
						push(n2,(octree::tree *)t->bbr.child,p.x,p.y,p.z,l/2);
						push(o,(octree::tree *)t->bbr.child,p.x,p.y,p.z,l/2);
						return;
					}
				}
			}
		}else{
			if((o->position.z)>(z+l)){
				//tbl
						p.init(x,y,z);
						p.z=z+l;
						o->octreebuffer.position.x=p.x;
						o->octreebuffer.position.y=p.y;
						o->octreebuffer.position.z=p.z;
						o->octreebuffer.length=l/2;
						o->octreebuffer.Tree=t;
						o->octreebuffer.deep++;
				if(t->tbl.child==null){
					t->tbl.isTree=false;
					t->tbl.child=o;
					return;
				}else{
					if(t->tbl.isTree){
						push(o,(octree::tree *)t->tbl.child,p.x,p.y,p.z,l/2);
						return;
					}else{
						n2=(octree::thing *)t->tbl.child;
						t->tbl.child=construct();
						t->tbl.isTree=true;
						push(n2,(octree::tree *)t->tbl.child,p.x,p.y,p.z,l/2);
						push(o,(octree::tree *)t->tbl.child,p.x,p.y,p.z,l/2);
						return;
					}
				}
			}else{
				//bbl
						p.init(x,y,z);
						o->octreebuffer.position.x=p.x;
						o->octreebuffer.position.y=p.y;
						o->octreebuffer.position.z=p.z;
						o->octreebuffer.length=l/2;
						o->octreebuffer.Tree=t;
						o->octreebuffer.deep++;
				if(t->bbl.child==null){
					t->bbl.isTree=false;
					t->bbl.child=o;
					return;

				}else{
					if(t->bbl.isTree){
						push(o,(octree::tree *)t->bbl.child,p.x,p.y,p.z,l/2);
						return;
					}else{
						n2=(octree::thing *)t->bbl.child;
						t->bbl.child=construct();
						t->bbl.isTree=true;
						push(n2,(octree::tree *)t->bbl.child,p.x,p.y,p.z,l/2);
						push(o,(octree::tree *)t->bbl.child,p.x,p.y,p.z,l/2);
						return;
					}
				}
			}
		}
	}
}
void 	octree::thing::update(){
	octree 	     *	p;
	octree::tree *	t;
	if(octreebuffer.Tree==null) return;
	p=octreebuffer.Tree->Pool->par;
	p->pt=	octreebuffer.Tree;
	p->pp.x=	octreebuffer.position.x;
	p->pp.y=	octreebuffer.position.y;
	p->pp.z=	octreebuffer.position.z;
	p->pl=	octreebuffer.length;
	if(p->p_isin(this)) return;
	if(p->pt->other==this)p->pt->other=next;
	if(last){last->next=next; last=null;}
	if(next){next->last=last; next=null;}
	if(p->pt->parent!=null)
		p->p_goparent();
	else
		return;
	while(1){
		if(p->pt->tfl.child==this){p->pt->tfl.child=null;}
		if(p->pt->tfr.child==this){p->pt->tfr.child=null;}
		if(p->pt->tbl.child==this){p->pt->tbl.child=null;}
		if(p->pt->tbr.child==this){p->pt->tbr.child=null;}
		if(p->pt->bfl.child==this){p->pt->bfl.child=null;}
		if(p->pt->bfr.child==this){p->pt->bfr.child=null;}
		if(p->pt->bbl.child==this){p->pt->bbl.child=null;}
		if(p->pt->bbr.child==this){p->pt->bbr.child=null;}
		//remove self node
		if(p->p_isin(this)){
			p->push(this);
			return;
		}
		if(p->pt->isEmpty()){
			t=p->pt;
			if(p->pt->parent!=null){
				p->p_goparent();
			}else{
				octreebuffer.Tree=null;
				t->destruct();
				return;
			}
			*(t->parent->posi(t->parposi))=null;
			t->destruct();
			continue;
		}
		if(p->pt->parent!=null)
			p->p_goparent();
		else
			return;
	}
}
void 	octree::thing::remove(){
	if(octreebuffer.Tree==null) return;
	if(octreebuffer.Tree->tfl.child==this){octreebuffer.Tree->tfl.child=null;}
	if(octreebuffer.Tree->tfr.child==this){octreebuffer.Tree->tfr.child=null;}
	if(octreebuffer.Tree->tbl.child==this){octreebuffer.Tree->tbl.child=null;}
	if(octreebuffer.Tree->tbr.child==this){octreebuffer.Tree->tbr.child=null;}
	if(octreebuffer.Tree->bfl.child==this){octreebuffer.Tree->bfl.child=null;}
	if(octreebuffer.Tree->bfr.child==this){octreebuffer.Tree->bfr.child=null;}
	if(octreebuffer.Tree->bbl.child==this){octreebuffer.Tree->bbl.child=null;}
	if(octreebuffer.Tree->bbr.child==this){octreebuffer.Tree->bbr.child=null;}
	if(octreebuffer.Tree->other==this){octreebuffer.Tree->other=next;}
	if(last)last->next=next;
	if(next)next->last=last;
	octreebuffer.Tree->trydest();
	octreebuffer.Tree=null;
}
template<typename T>
void octree::foreach(T *c){
	foreachchild(pt,pp.x,pp.y,pp.z,pl,c);
}
template<typename T>
void octree::foreachchild(octree::tree *t,t3dfloat x,t3dfloat y,t3dfloat z,t3dfloat l,T * c){
	vec3<t3dfloat> p;
	thing *	tt;
	if(t==null) return;
	if(!c->foreachnode(t,x,y,z,l,c)) return;
	if(t->other){
		tt=t->other;
		while(tt){
			c->foreachthing(tt,x,y,z,l);
			tt=tt->next;
		}
	}
		// _TFL
		if(t->tfl.child!=null){
			p.init(x,y,z);
			p.x=x+l;
			p.z=z+l;
		if(t->tfl.isTree){
			foreachchild((octree::tree *)t->tfl.child ,p.x,p.y,p.z,l/2,c);
		}else{
			c->foreachthing((octree::thing *)t->tfl.child ,p.x,p.y,p.z,l/2);
		}
		}
		//
		// _TFR :
		if(t->tfr.child!=null){
			p.init(x,y,z);
			p.x=x+l;
			p.y=y+l;
			p.z=z+l;
		if(t->tfr.isTree){
			foreachchild((octree::tree *)t->tfr.child ,p.x,p.y,p.z,l/2,c);
		}else{
			c->foreachthing((octree::thing *)t->tfr.child ,p.x,p.y,p.z,l/2);
		}
		}
		//
		// _TBL :
		if(t->tbl.child!=null){
			p.init(x,y,z);
			p.z=z+l;
		if(t->tbl.isTree){
			foreachchild((octree::tree *)t->tbl.child ,p.x,p.y,p.z,l/2,c);
		}else{
			c->foreachthing((octree::thing *)t->tbl.child ,p.x,p.y,p.z,l/2);
		}
		}
		//
		// _TBR :
		if(t->tbr.child!=null){
			p.init(x,y,z);
			p.y=y+l;
			p.z=z+l;
		if(t->tbr.isTree){
			foreachchild((octree::tree *)t->tbr.child ,p.x,p.y,p.z,l/2,c);
		}else{
			c->foreachthing((octree::thing *)t->tbr.child ,p.x,p.y,p.z,l/2);
		}
		}
		//
		// _BFL :
		if(t->bfl.child!=null){
			p.init(x,y,z);
			p.x=x+l;
		if(t->bfl.isTree){
			foreachchild((octree::tree *)t->bfl.child ,p.x,p.y,p.z,l/2,c);
		}else{
			c->foreachthing((octree::thing *)t->bfl.child ,p.x,p.y,p.z,l/2);
		}
		}
		//
		// _BFR :
		if(t->bfr.child!=null){
			p.init(x,y,z);
			p.x=x+l;
			p.y=y+l;
		if(t->bfr.isTree){
			foreachchild((octree::tree *)t->bfr.child ,p.x,p.y,p.z,l/2,c);
		}else{
			c->foreachthing((octree::thing *)t->bfr.child ,p.x,p.y,p.z,l/2);
		}
		}
		//
		// _BBL :
		if(t->bbl.child!=null){
			p.init(x,y,z);
		if(t->bbl.isTree){
			foreachchild((octree::tree *)t->bbl.child ,p.x,p.y,p.z,l/2,c);
		}else{
			c->foreachthing((octree::thing *)t->bbl.child ,p.x,p.y,p.z,l/2);
		}
		}
		//
		// _BBR :
		if(t->bbr.child!=null){
			p.init(x,y,z);
			p.y=y+l;
		if(t->bbr.isTree){
			foreachchild((octree::tree *)t->bbr.child ,p.x,p.y,p.z,l/2,c);
		}else{
			c->foreachthing((octree::thing *)t->bbr.child ,p.x,p.y,p.z,l/2);
		}
		}
}
template<typename T>
void octree::search(t3dfloat x,t3dfloat y,t3dfloat z,t3dfloat l,T *c){
	struct searchcallback{
		void	foreachthing	(octree::thing *t,t3dfloat x,t3dfloat y,t3dfloat z,t3dfloat l){
			cb->callback(t);
		}
		bool 	foreachnode	(octree::tree  *t,t3dfloat x,t3dfloat y,t3dfloat z,t3dfloat l,searchcallback *c){
			if(searchPosition.x+r>x)
			if(searchPosition.y+r>y)
			if(searchPosition.z+r>z)
			if(searchPosition.x+r<x)
			if(searchPosition.y+r<y)
			if(searchPosition.z+r<z)
			return true;
			return false;
		}
		T *cb;
		vec3<t3dfloat>	searchPosition;
		t3dfloat tx,ty,tz,r;
	}ca;
	ca.cb=c;
	ca.searchPosition.init(x,y,z);
	ca.r=l;
	foreach(&ca);
}
#endif
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
#ifndef _t3dSpace
#define _t3dSpace
#define floatmod(x,y) x-((int)(x/y))*y
namespace physical{
	void  rotatePoint(vec3<t3dfloat> *,vec3<t3dfloat> *,vec3<t3dfloat> *);
	class object;
	class sphere;
	class Shape;
	class collider;
	class space;
	class world;
	class clog{
		public:
			void *	text;
			clog * 	next;
	};
	class clogpool{
		friend class space;
		public:
		protected:
			clog 		c[_t3dCollidePoolSize];
			space *		par;
			clogpool *	next;
			int		p;
			int		time;
			clogpool(){
				p=0;
				time=0;
				next=null;
			}
			~clogpool(){
				if(next) delete next;
			}
			clog *	construct();
	};
	class space{
		friend class collider;
		friend class world;
		public:
			t3dfloat	 	length;
			int 			time;
			t3dfloat		timestep;
			bool(*onCollideWall)(object *o);
			t3dcallback4		collided;
			space():objects(),oct(),colBuffer(){
				collided=null;
				time=0;
				timestep=1.0f;
				logpool=new clogpool;
				logpool2=new clogpool;
				logpool->par=this;
				logpool2->par=this;
				maxR=0.0f;
			}
			~space(){
				delete logpool;
				delete logpool2;
			}
			void add(object *o);
			void remove(object *o);
			void update(object *o);
			void nextstep();
			void maxRinit();
			bool collideLog(sphere * s,void * t);
		protected:
			octree 			oct;
			octree			colBuffer;
			clogpool	*	logpool;
			clogpool	*	logpool2;
			t3dfloat 		maxR;
			tmp::table<object*> 	objects;
			void collide(object *o);
	};
	//class Cyliner{
	//	public:
	//		vec3<t3dfloat> 	center;
	//		t3dfloat 	r;
	//};
	class sphereBase{
		public:
			t3dfloat	 r;
			vec3<t3dfloat> 	 position;
			sphereBase():position(){
				r=0.0f;
			}
			bool isCollide(
				t3dfloat x,
				t3dfloat y,
				t3dfloat z,
				t3dfloat R
			);
	};
	class sphere:public sphereBase{
		public:
			//bool 		cylinerMode;
			//Cyliner	cyliner;
			sphere(){
				time=0;
				collideLog=null;
				collideLogLast=null;
			}
			void operator=(sphere ss){
				r=ss.r;
				position=&ss.position;
			}
			int	time;
			clog * 	collideLog;
			clog * 	collideLogLast;
	};
	class Shape{
		friend class collider;
		friend class space;
		public:
			sphere *		spheres;
			sphere 			mainSphere;
			int 			num;
			void setCenter(t3dfloat x,t3dfloat y,t3dfloat z);
			Shape():mainSphere(){
				spheres=null;
			}
			~Shape(){
				if(spheres) delete[] spheres;
			}
			void setShape();
			void maxRinit();
			class creation{
				public:
					creation():sph(){
					}
					void addsphere(t3dfloat r,vec3<t3dfloat> * p){
						sphere spw;
						spw.position=p;
						spw.r=r;
						sph.push(spw);
					}
					void output(Shape * sh){
						struct cb{
							Shape * sh;
							int 	i;
						}c;
						c.i=0;
						c.sh=sh;
						sh->num=sph.length();
						sh->spheres=new sphere[sh->num];
						sph.foreach([](sphere * sp,void *a){
							cb *c=(cb*)a;
							c->sh->spheres[c->i]=*sp;
							c->i++;
						},&c);
						sh->maxRinit();
					}
				private:
					tmp::array<sphere> 	sph;
					t3dfloat		maxR;
			};
		protected:
			t3dfloat 		maxR;
	};
	class Bsphere:public sphereBase{
		public:
			sphere *	parent;
			octree::thing	node;
			Bsphere():node(){
				node.child=this;
			}
			void init(sphere *s);
			void update();
	};
	class object{
		friend class space;
		friend class collider;
		public:
			bool 				sta;		//is static
			bool 				stalock;
			bool 				hidden;
			t3dfloat 			m;
			t3dfloat			I;
			vec3<t3dfloat> 			position;
			vec3<t3dfloat> 			rotation;
			Shape	*			shape;
			bool				(*onCollide)(object*,object*,sphere*,sphere*);
			vec3<t3dfloat>			move;		//speed of moving
			vec3<t3dfloat>			rotate;		//speed of rotation
			octree::thing 			parent;
			object(Shape *s):position(),rotation(),shape(),parent(){
				sta		=false;
				stalock		=false;
				hidden 		=false;
				time 		=0;
				parent.child	=this;
				onCollide	=null;
				shape		=s;
			}
			void isCollide(object * a,void (*callback)(object * b));
			void setPosition(t3dfloat x,t3dfloat y,t3dfloat z);
			void setRotation(t3dfloat x,t3dfloat y,t3dfloat z);
			void setMove(t3dfloat x,t3dfloat y,t3dfloat z);
			void setRotate(t3dfloat x,t3dfloat y,t3dfloat z);
			void getVelocity(vec3<t3dfloat> *p,vec3<t3dfloat> *out);
			void addimpulse(vec3<t3dfloat> *p,vec3<t3dfloat> *i);
		protected:
			int 		time;
			vec3<t3dfloat>	moveAdd;
			vec3<t3dfloat>	rotateAdd;
			Bsphere *	sphBuffer;
			octree::tree *	oct;
			void bufferinit();
			void bufferupdate();
	};
	class collider{
		friend class space;
		public:
			space 	*	parent;
			bool		oncemode;
			collider(space *s){
				o=null;
				cb.parent=this;
				parent=s;
				ob=octree(&(s->colBuffer));
			}
			struct colcb{
				collider *	parent;
				void 		callback(octree::thing *t);
			}cb;
			void start	();
			void setObject	(object *obj){
				o=obj;
			}
		protected:
			object 		*o;
			octree		ot;//global octree pointer
			octree		ob;//object octree pointer
			bool isin	(vec3<t3dfloat> *p);
			void buildoctree(object *obj);
			void collideWall();
			void step1	();
			void step2	();
			void step3	(object *obj);
			void step4	(object *a,object *b);
			void collided	(object *,object *,sphere *,sphere *);
	};
//////////////////////////////////////
//////////////////////////////////////
//////Functions Begin/////////////////
//////////////////////////////////////
//////////////////////////////////////
void Shape::setCenter(t3dfloat x,t3dfloat y,t3dfloat z){
	int i;
	for(i=0;i<num;i++){
		spheres[i].position.x-=x;
		spheres[i].position.y-=y;
		spheres[i].position.z-=z;
	}
	maxRinit();
}
void Shape::maxRinit(){
	int 		i;
	t3dfloat 	l;
	maxR=0.0f;
	for(i=0;i<num;i++){
		l=spheres[i].position.norm()+spheres[i].r;
		if(l>maxR)
			maxR=l;
	}
	mainSphere.r=maxR;
}
//////////////////////////////////////
//////collide  begin//////////////////
//////////////////////////////////////
clog *	clogpool::construct(){
	if(time!=par->time){
		p=0;
	}
	if(p>_t3dCollidePoolSize){
		if(next==null){
			next=new clogpool;
			next->time=time;
			next->par=par;
		}
		return	next->construct();
	}
	p++;
	c[p-1].next=null;
	return &c[p-1];
}
bool space::collideLog(sphere * s,void * t){
	clog *	c;
	bool	res;
	if(s->time!=time){
		s->collideLogLast=s->collideLog;
		s->collideLog=logpool->construct();
		s->collideLog->text=t;
		return false;
	}
	if(s->collideLogLast){
		c=s->collideLogLast;
		while(1){
			if(c->text==s){
				res=true;
				break;
			}
			if(c==null)
				break;
			c=c->next;
		}
	}
	c=s->collideLog;
	while(1){
		if(c->text==t)
			return true;
		if(c->next==null){
			c->next=logpool->construct();
			c->next->text=t;
			return res;
		}
		c=c->next;
	}
}
void collider::collided(
	object * a,
	object * b,
	sphere * oa,
	sphere * ob
){
	if(parent->collideLog(oa,ob))return;
	if(parent->collideLog(ob,oa))return;
	if(a->stalock)a->sta=true; else a->sta=false;
	if(b->stalock)b->sta=true; else b->sta=false;
	if(a->onCollide) a->onCollide(a,b,oa,ob);
	if(b->onCollide) b->onCollide(a,b,oa,ob);
	if(parent->collided){
		t3dcallback4 cb;
		cb=(t3dcallback4)parent->collided(a,b,oa,ob);
		if(cb){
			((t3dcallback4)cb)(a,b,oa,ob);
			return;
		}
	}
	if(a->stalock || b->stalock || a->hidden || b->hidden)return;
	vec3<t3dfloat> 	buf1,buf2;
	t3dfloat 	buf3,buf4,buf5,buf6;
	//get true position of sphere
	vec3<t3dfloat> pa(
		oa->position.x+a->position.x,
		oa->position.y+a->position.y,
		oa->position.z+a->position.z
	);
	rotatePoint(
		&pa,
		&(a->position),
		&(a->rotation)
	);
	vec3<t3dfloat> pb(
		ob->position.x+b->position.x,
		ob->position.y+b->position.y,
		ob->position.z+b->position.z
	);
	rotatePoint(
		&pb,
		&(b->position),
		&(b->rotation)
	);
	//end
	//get normal line
	vec3<t3dfloat> normal(
		pa.x-pb.x,
		pa.y-pb.y,
		pa.z-pb.z
	);
	buf3=sqrt(
		(normal.x*normal.x)+
		(normal.y*normal.y)+
		(normal.z*normal.z)
	);
	normal.x=normal.x/buf3;
	normal.y=normal.y/buf3;
	normal.z=normal.z/buf3;
	//end
	//get point
	buf1=&normal;
	vec3<t3dfloat> normal2(
		-buf1.x,
		-buf1.y,
		-buf1.z
	);
	buf1.init(
		normal2.x*oa->r,
		normal2.y*oa->r,
		normal2.z*oa->r
	);
	buf2.init(
		normal.x*ob->r,
		normal.y*ob->r,
		normal.z*ob->r
	);
	vec3<t3dfloat> point(
		(buf1.x+buf2.x)/2.0f,
		(buf1.y+buf2.y)/2.0f,
		(buf1.z+buf2.z)/2.0f
	);
	//end
	//get velocity
	vec3<t3dfloat> va,vb;
	a->getVelocity(&point,&va);
	b->getVelocity(&point,&vb);
	//end
	//comput energy
	vec3<t3dfloat> v(
		a->move.x-b->move.x,
		a->move.y-b->move.y,
		a->move.z-b->move.z
	);
	vec3<t3dfloat> Pv(
		v.x*a->m,
		v.y*a->m,
		v.z*a->m
	);
	t3dfloat ra=sqrt((point.x-a->position.x)*(point.x-a->position.x)+(point.y-a->position.y)*(point.y-a->position.y)+(point.z-a->position.z)*(point.z-a->position.z));
	t3dfloat rb=sqrt((point.x-b->position.x)*(point.x-b->position.x)+(point.y-b->position.y)*(point.y-b->position.y)+(point.z-b->position.z)*(point.z-b->position.z));
	vec3<t3dfloat> Pra(
		a->rotate.x*a->I/ra,
		a->rotate.y*a->I/ra,
		a->rotate.z*a->I/ra
	);
	vec3<t3dfloat> Prb(
		b->rotate.x*b->I/rb,
		b->rotate.y*b->I/rb,
		b->rotate.z*b->I/rb
	);
	vec3<t3dfloat> P(
		Pra.x-Prb.x+Pv.x,
		Pra.y-Prb.y+Pv.y,
		Pra.z-Prb.z+Pv.z
	);
	t3dfloat dpv=P.x*normal.x+P.y*normal.y+P.z*normal.z;
	vec3<t3dfloat> dP(
		normal.x*dpv,
		normal.y*dpv,
		normal.z*dpv
	);
	//end
	vec3<t3dfloat> dP2(
		-dP.x,
		-dP.y,
		-dP.z
	);
	a->addimpulse(&point,&dP2);
	b->addimpulse(&point,&dP );
}
t3dfloat p2i(t3dfloat * center,t3dfloat *point,t3dfloat *i){
	t3dfloat r,d,out;
	r=point[0]-center[0];
	d=i[1]*r;
	if(r>0)
		out= d;
	else
		out=-d;
	r=point[1]-center[1];
	d=i[0]*r;
	if(r>0)
		out+=-d;
	else
		out+= d;
	return out;
}
void object::addimpulse(vec3<t3dfloat> *p,vec3<t3dfloat> *i){
	t3dfloat buf1,buf2;
	//t3dfloat r;
	t3dfloat bv1[1];
	t3dfloat bv2[1];
	t3dfloat bv3[1];
	vec3<t3dfloat> buf3,buf4;
	vec3<t3dfloat> normal(
		p->x-position.x,
		p->y-position.y,
		p->z-position.z
	);
	buf1=normal.norm();
	normal.x=normal.x/buf1;
	normal.y=normal.y/buf1;
	normal.z=normal.z/buf1;
	buf1=i->x*normal.x+i->y*normal.y+i->z*normal.z;
	vec3<t3dfloat> i1(
		normal.x*buf1,
		normal.y*buf1,
		normal.z*buf1
	);
	vec3<t3dfloat> i2(
		i->x-i1.x,
		i->y-i1.y,
		i->z-i1.z
	);
	moveAdd.x+=i1.x/m;
	moveAdd.y+=i1.y/m;
	moveAdd.z+=i1.z/m;
	//buf3.x=i->x+p->x;
	//buf3.y=i->y+p->y;
	//buf3.z=i->z+p->z;
	//buf3.x=buf3.x-position.x;
	//buf3.y=buf3.y-position.y;
	//buf3.z=buf3.z-position.z;
	//buf3.init(
	//	buf3.y * i->z - buf3.z * i->y,
	//	buf3.z * i->x - buf3.x * i->z,
	//	buf3.z * i->y - buf3.y * i->x
	//);
	//r=buf3.norm();
	//r=r/i->norm();

	//yx
	bv1[0]=position.y;
	bv1[1]=position.x;
	bv2[0]=p->y;
	bv2[1]=p->x;
	bv3[0]=i->y;
	bv3[1]=i->x;
	rotateAdd.z+=p2i(bv1,bv2,bv3);
	//zy
	bv1[0]=position.z;
	bv1[1]=position.y;
	bv2[0]=p->z;
	bv2[1]=p->y;
	bv3[0]=i->z;
	bv3[1]=i->y;
	rotateAdd.x+=p2i(bv1,bv2,bv3);
	//xz
	bv1[0]=position.x;
	bv1[1]=position.z;
	bv2[0]=p->x;
	bv2[1]=p->z;
	bv3[0]=i->x;
	bv3[1]=i->z;
	rotateAdd.y+=p2i(bv1,bv2,bv3);
}
void getRtLine2d(t3dfloat * in,t3dfloat * out){
	t3dfloat n=sqrt(in[0]*in[0]+in[1]*in[1]);
	out[0]= in[1];
	out[1]=-in[0];
	if(n==0)
		return;
	out[0]=out[0]/n;
	out[1]=out[1]/n;
	//if(in[1]==0.0f){
	//	if(in[0]==0.0f){
	//		out[1]=0.0f;
	//		out[0]=0.0f;
	//		return;
	//	}
	//	if(in[0]<0.0f){
	//		out[0]=0.0f;
	//		out[1]=1.0f;
	//		return;
	//	}else{
	//		out[0]=0.0f;
	//		out[1]=-1.0f;
	//		return;
	//	}
	//}
	//if(in[0]==0.0f){
	//	if(in[1]>0.0f){
	//		out[0]=1.0f;
	//		out[1]=0.0f;
	//		return;
	//	}else{
	//		out[0]=-1.0f;
	//		out[1]=0.0f;
	//		return;
	//	}
	//}
	//t3dfloat b=in[1]/in[0];
	//out[1]=sqrt(b*b+1.0f);
	//out[0]=-out[1]*b;
	//if(in[1]<0.0f)
	//	return;
	//else{
	//	out[1]=-out[1];
	//	out[0]=-out[0];
	//}
}
void getVelocity2d(t3dfloat * p,t3dfloat * c,t3dfloat w,t3dfloat * out){
	t3dfloat line1[1];
	t3dfloat line2[1];
	t3dfloat r,v;
	line1[0]=p[0]-c[0];
	line1[1]=p[1]-c[1];
	if(line1[0]==0.0f)
	if(line1[1]==0.0f){
		out[0]=0.0f;
		out[1]=0.0f;
		return;
	}
	r=sqrt(line1[0]*line1[0]+line1[1]*line1[1]);
	v=w/r;
	getRtLine2d(line1,line2);
	if(w<0){
		out[0]=line2[0]*v;
		out[1]=line2[1]*v;
	}else{
		out[0]=-line2[0]*v;
		out[1]=-line2[1]*v;
	}
}
void object::getVelocity(vec3<t3dfloat> *in,vec3<t3dfloat> *out){
	t3dfloat buf [1];
	t3dfloat bufp[1];
	t3dfloat bufo[1];
	out->x=0.0f;
	out->y=0.0f;
	out->z=0.0f;
	//yx
	buf [0]=in->y;
	buf [1]=in->x;
	bufp[0]=position.y;
	bufp[1]=position.x;
	getVelocity2d(buf,bufp,rotate.z,bufo);
	out->y+=bufo[0];
	out->x+=bufo[1];
	//zy
	buf [0]=in->z;
	buf [1]=in->y;
	bufp[0]=position.z;
	bufp[1]=position.y;
	getVelocity2d(buf,bufp,rotate.x,bufo);
	out->z+=bufo[0];
	out->y+=bufo[1];
	//xz
	buf [0]=in->x;
	buf [1]=in->z;
	bufp[0]=position.x;
	bufp[1]=position.z;
	getVelocity2d(buf,bufp,rotate.y,bufo);
	out->x+=bufo[0];
	out->z+=bufo[1];

	out->x=out->x/2.0f;
	out->y=out->x/2.0f;
	out->z=out->x/2.0f;
}
//////////////////////////////////////
//////collide  end////////////////////
//////////////////////////////////////
void rotatePoint(
	vec3<t3dfloat> *point,
	vec3<t3dfloat> *center,
	vec3<t3dfloat> *Rotate
){
	vec3<t3dfloat> 	buf(0.0f , 0.0f , 0.0f);
	vec3<t3dfloat> 	r(point->x - center->x , point->y - center->y , point->z - center->z);
	t3dfloat cosx=cos(-(Rotate->x));
	t3dfloat cosy=cos(-(Rotate->y));
	t3dfloat cosz=cos(-(Rotate->z));
	t3dfloat sinx=sqrt(1-cosx*cosx);
	t3dfloat siny=sqrt(1-cosy*cosy);
	t3dfloat sinz=sqrt(1-cosz*cosz);
	buf.x	=r.x*cosz-r.y*sinz;
	buf.y	=r.x*sinz+r.y*cosz;
	r.x	=buf.x;
	r.y	=buf.y;
	buf.z	=r.z*cosy-r.x*siny;
	buf.x	=r.z*siny+r.x*cosy;
	r.z	=buf.z;
	r.x	=buf.x;
	buf.y	=r.y*cosx-r.z*sinx;
	buf.z	=r.y*sinx+r.z*cosx;
	point->x=buf.x;
	point->y=buf.y;
	point->z=buf.z;
}
void space::add(object *o){
	objects.insert(o);
	oct.push(&o->parent);
	o->bufferinit();
	if(o->shape->maxR>maxR)
		maxR=o->shape->maxR;
	this->collide(o);
}
void space::remove(object *o){
	o->parent.remove();
	objects.deldata(o);
}
void space::update(object *o){
	o->parent.position.x=o->position.x;
	o->parent.position.y=o->position.y;
	o->parent.position.z=o->position.z;
	o->parent.update();
}
void space::maxRinit(){
	maxR=0.0f;
	objects.foreach([](tmp::table<object*>::info *info,void *s){
		space * self=(space*)s;
		t3dfloat R=(*(info->data))->shape->maxR;
		if(R>self->maxR) self->maxR=R;
	},this);
}
struct nfset{
	space 	*	sp;
};
void space::nextstep(){
	nfset 		n;
	clogpool *	c;
	c=logpool;
	logpool=logpool2;
	logpool2=c;
	n.sp=this;
	time++;
	objects.foreach([](tmp::table<object*>::info *info,void *s){
		nfset 	*self=(nfset*)s;
		object 	*o;
		o=*(info->data);
		if(o->sta || o->stalock){
			o->rotateAdd.init(0.0f,0.0f,0.0f);
			o->moveAdd.init(0.0f,0.0f,0.0f);
			return;
		}
		o->move.x+=o->moveAdd.x;
		o->move.y+=o->moveAdd.y;
		o->move.z+=o->moveAdd.z;
		o->rotate.x+=o->rotateAdd.x;
		o->rotate.y+=o->rotateAdd.y;
		o->rotate.z+=o->rotateAdd.z;
		o->rotateAdd.init(0.0f,0.0f,0.0f);
		o->moveAdd.init(0.0f,0.0f,0.0f);
		o->position.x+=(o->move.x)*(self->sp->timestep);
		o->position.y+=(o->move.y)*(self->sp->timestep);
		o->position.z+=(o->move.z)*(self->sp->timestep);
		o->rotation.x+=(o->rotate.x)*(self->sp->timestep);
		o->rotation.y+=(o->rotate.y)*(self->sp->timestep);
		o->rotation.z+=(o->rotate.z)*(self->sp->timestep);
		o->rotation.x=floatmod(o->rotation.x,2*pai);
		o->rotation.y=floatmod(o->rotation.y,2*pai);
		o->rotation.z=floatmod(o->rotation.z,2*pai);
	},&n);
	objects.foreach([](tmp::table<object*>::info *info,void *s){
		nfset *self=(nfset*)s;
		self->sp->collide(*(info->data));
	},&n);
}
void object::setPosition(t3dfloat x,t3dfloat y,t3dfloat z){
	position.x=x;
	position.y=y;
	position.z=z;
}
void object::setRotation(t3dfloat x,t3dfloat y,t3dfloat z){
	rotation.x=x;
	rotation.y=y;
	rotation.z=z;
}
void object::setMove(t3dfloat x,t3dfloat y,t3dfloat z){
	move.x=x;
	move.y=y;
	move.z=z;
}
void object::setRotate(t3dfloat x,t3dfloat y,t3dfloat z){
	rotation.x=x;
	rotation.y=y;
	rotation.z=z;
}
void collider::colcb::callback(octree::thing *t){
	object *	o;
	o=(object*)t->child;
	if(o==parent->o) return;
	if(parent->oncemode)
		if(o < (parent->o))
			return;
	parent->step3(o);
}
bool collider::isin(vec3<t3dfloat> *p){
	if(
		p->x>ot.pp.x ||
		p->y>ot.pp.y ||
		p->z>ot.pp.z ||
		p->x<ot.pp.x+ot.pl ||
		p->y<ot.pp.y+ot.pl ||
		p->z<ot.pp.z+ot.pl
	)
		return true;
	else
		return false;
}
void collider::start(){
	step1();
	step2();
}
void collider::step1(){
	vec3<t3dfloat> bb,be,pp;
	t3dfloat 	ll,rr;
	if((o->hidden && !o->onCollide) || o->sta)
		return;
	ll=parent->oct.length;
	pp=&o->position;
	rr=o->shape->mainSphere.r;
	if(
		pp.x+rr>ll ||
		pp.y+rr>ll ||
		pp.z+rr>ll ||
		pp.x-rr>0.0f ||
		pp.y-rr>0.0f ||
		pp.z-rr>0.0f
	)
		if(parent->onCollideWall(o))
			return;
	sphere	*s=&(o->shape->mainSphere);
	bb.init(
		(o->position.x)+(s->r),
		(o->position.y)+(s->r),
		(o->position.z)+(s->r)
	);
	be.init(
		(o->position.x)-(s->r),
		(o->position.y)-(s->r),
		(o->position.z)-(s->r)
	);
	ot=o->parent.octreebuffer.Tree->Pool->par;
	while(1){
		if(ot.pt->parent==null)
			break;
		if(isin(&bb))
		if(isin(&be))
			break;
		ot.p_goparent();
	}
}
void collider::step2(){
	sphere	*s=&(o->shape->mainSphere);
	ot.search(
		o->position.x,
		o->position.y,
		o->position.z,
		s->r+parent->maxR,
		&cb
	);
}
void collider::step3(object *obj){
	//obj collide with this->o
	vec3<t3dfloat> p(
		(o->position.x)-(obj->position.x),
		(o->position.y)-(obj->position.y),
		(o->position.z)-(obj->position.z)
	);
	if(sqrt(
		(p.x*p.x)+
		(p.y*p.y)+
		(p.z*p.z)
	)>((o->shape->mainSphere.r)+(obj->shape->mainSphere.r)))
		return;
	if(parent->time==o->time)
		step4(o,obj);
	else
	if(parent->time==obj->time)
		step4(obj,o);
	else{
		buildoctree(o);
		step4(o,obj);
	}
}
void collider::step4(object *a,object *b){
	int i,l;
	t3dfloat addx,addy,addz;
	struct findintwothing{
		collider * par;
		void callback(octree::thing *t){
			spa=((Bsphere *)(t->child))->parent;
			if(spb==spa)return;
			if(!spb->isCollide(
				spa->position.x,
				spa->position.y,
				spa->position.z,
				spa->r
			))return;
			par->collided(a,b,spa,spb);
		}
		object *a;
		object *b;
		sphere *spa;
		sphere *spb;
	}fitt;
	fitt.a=a;
	fitt.b=b;
	fitt.par=this;
	b->bufferupdate();
	Bsphere *sp=b->sphBuffer;
	l=b->shape->num;
	octree::tree *t=ob.root;
	ob.root=a->oct;
	ob.length=a->shape->mainSphere.r*2.0f;
	addx=a->position.x-a->shape->mainSphere.r;
	addy=a->position.y-a->shape->mainSphere.r;
	addz=a->position.z-a->shape->mainSphere.r;
	//search begin
	for(i=0;i<=l;i++){
		fitt.spb=sp[i].parent;
		ob.search(
			sp[i].position.x-addx,
			sp[i].position.y-addy,
			sp[i].position.z-addz,
			sp[i].r+parent->maxR,
			&fitt
		);
	}
	//search end
	ob.root=t;
}
void collider::buildoctree(object *obj){
	int l,i;
	Bsphere *sp=obj->sphBuffer;
	obj->time=parent->time;
	obj->bufferupdate();
	l=obj->shape->num;
	if(ob.root==null)
	ob.root=ob.construct();
	//ob.root
	ob.length=obj->shape->mainSphere.r*2.0f;
	ob.p_init();
	for(i=0;i<=l;i++){
		ob.push(&(sp[i].node));
	}
	obj->oct=ob.root;
}
void object::bufferinit(){
	sphere *sh;
	int l,i;
	sh=shape->spheres;
	l= shape->num;
	sphBuffer=new Bsphere[l];
	for(i=0;i<=l;i++){
		sphBuffer[i].init(&sh[i]);
	}
	bufferupdate();
}
void object::bufferupdate(){
	int i,l;
	Bsphere *s1;
	sphere  *s2;
	sphere  *sh=shape->spheres;
	l= shape->num;
	for(i=0;i<l;i++){
		s1=&sphBuffer[i];
		s2=&sh[i];
		s1->position=&s2->position;
		s1->position.x=position.x+s1->parent->position.x;
		s1->position.y=position.y+s1->parent->position.y;
		s1->position.z=position.z+s1->parent->position.z;
		rotatePoint(
			&s1->position,
			&position,
			&rotation
		);
		s1->update();
		s1->node.position.x-=position.x-shape->mainSphere.r;
		s1->node.position.y-=position.y-shape->mainSphere.r;
		s1->node.position.z-=position.z-shape->mainSphere.r;
	}
}
void Bsphere::init(sphere *s){
	parent=s;
	r=s->r;
}
void Bsphere::update(){
	node.position=&position;
}
bool sphereBase::isCollide(
	t3dfloat x,
	t3dfloat y,
	t3dfloat z,
	t3dfloat R
){
	t3dfloat X,Y,Z,rr;
	X=x-position.x;
	Y=y-position.y;
	Z=z-position.z;
	rr=sqrt((X*X)+(Y*Y)+(Z*Z));
	if(rr<(R+r))
		return true;
	else
		return false;
}
void space::collide(object *o){
	collider col(this);
	col.oncemode=true;
	col.setObject(o);
	col.start();
}
//////////////////////////////////////
//////////////////////////////////////
//////Functions End///////////////////
//////////////////////////////////////
//////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
class world{
	public:
		space			Space;
		tmp::table<object*> *	objects;
		void setOctreeDeep(int n){
			Space.oct.maxdeep=n;
		}
		world(t3dfloat l):Space(){
			length=l;
			Space.length=l;
			Space.oct.length=l;
			objects=&(Space.objects);
		}
	private:
		t3dfloat 	length;
};
///////////////////////////////////////
///////////////////////////////////////
///////////////////////////////////////
}
#endif
///////////////////////////////////////////////////////////////
}
#endif
