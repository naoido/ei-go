<template>
    <div class="w-100 h-100 d-flex justify-center align-center flex-column">
        <v-card class="d-flex justify-center align-center flex-column pa-10 w-75">
            <v-card-title class="text-h2">
                最終結果
            </v-card-title>
            <div v-for="(rank, index) in top">
                <v-card flat>
                    <div class="d-flex justify-center align-center flex-row">
                        <v-icon size="50" :color="getColor(index)">mdi-crown</v-icon>
                        <v-card-title class="text-h3">
                            {{ users.find(u => u.id  === rank.id)?.username }}({{ rank.point }})
                        </v-card-title>
                        <v-icon size="50" :color="getColor(index)">mdi-crown</v-icon>
                    </div>
                </v-card>
            </div>

            <v-btn class="mt-10 w-25" color="success" @click="router.push('/')">終了</v-btn>
        </v-card>
    </div>
</template>

<script setup lang="ts">
import router from '@/router';


type User = {
    id: string,
    username: string,
    isReady: boolean,
    judging: boolean,
    isSuccessed: boolean | null,
    point: number,
}

type Rank = {
    id: string,
    point: number
}

type Props = {
    users: User[],
    top: Rank[]
}

const props = defineProps<Props>();
const getSortedUser = () => [...props.users].sort((a, b) => b.point - a.point);
const getColor = (rank: number): string => ["yellow-darken-2", "blue-grey-lighten-1", "brown-darken-2"][rank]
</script>